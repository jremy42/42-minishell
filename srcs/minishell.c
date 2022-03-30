/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 09:38:42 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	g_rv;

int	__get_pos_last_dir(char *cwd)
{
	int i;

	i = 0;
	while (cwd[i])
		i++;
	while (cwd[i] != '/' && i >= 0)
		i--;
	i++;
	return (i);
}

char	*__get_prompt(t_msh *msh)
{
	char	path[PATH_MAX];

	__bzero(path, PATH_MAX);
	free(msh->prompt);
	if (msh->rv == 0)
		msh->prompt = __strdup(BOLDGREEN"➜  "RESET BOLDCYAN);
	else
		msh->prompt = __strdup(BOLDRED"➜  "RESET BOLDCYAN);
	if (!msh->prompt)
		return(__exit_error(msh, 3, "get prompt"), NULL);
	if (getcwd(path, PATH_MAX))
	{
		msh->prompt = __strjoin2(msh->prompt,
			path + __get_pos_last_dir(path), RESET BOLDYELLOW "  ~  "RESET);
		if (!msh->prompt)
			return(__exit_error(msh, 3, "get prompt"), NULL);
	}
	else
	{
		msh->prompt = __strjoin(msh->prompt, RESET);
		if (!msh->prompt)
			return(__exit_error(msh, 3, "get prompt"), NULL);
	}
	return (msh->prompt);
}

int	get_size_env(char *envp[])
{
	int size;
	
	size = 0;
	if (!envp)
		return (0);
	while (envp[size])
		size++;
	return (size);
}
void	partial_destroy_env(t_msh *msh, int limit)
{
	int	i;

	i = 0;
	if (!msh->envp)
		return ;
	while (i < limit)
	{
		free(msh->envp[i][0]);
		free(msh->envp[i][1]);
		free(msh->envp[i]);
		i++;
	}
	free(msh->envp);
}

int	get_env(t_msh *msh, char *envp[])
{
	int	i;
	int size;

	i = -1;
	size = get_size_env(envp);
	msh->envp = (char ***)malloc((size + 1) * sizeof(char **));
	if (!msh->envp)
		return (0);
	msh->envp[size] = NULL;
	while (++i < size)
	{
		msh->envp[i] = (char **)malloc((3) * sizeof(char *));
		if(!msh->envp[i])
			return (partial_destroy_env(msh, i), 0);
		msh->envp[i][0] = __strdup(envp[i]);
		if(!msh->envp[i][0])
			return (free(msh->envp[i]), partial_destroy_env(msh, i), 0);
		msh->envp[i][1] = __strdup("1");
		if(!msh->envp[i][1])
			return (free(msh->envp[i][0]), free(msh->envp[i]), partial_destroy_env(msh, i), 0);
		msh->envp[i][2] = NULL;
	}
	if (update_pwd(msh) == __MALLOC)
		__exit_error(msh, 240, "Malloc Error in update pwd");
	if (update_shlvl(msh) == __MALLOC)
		__exit_error(msh, 240, "Malloc Error in update shlvl");
	return (1);
}

int __check_input(char *arg, char **to_tokenize, t_msh *msh)
{
	msh->syntax_error = 0;
	if(!__strcmp(arg,""))
		return(0);
	*to_tokenize = __strtrim(arg, " \f\t\r\v");
	if (!*to_tokenize)
		return (__exit_error(msh, 3, "check input"));
	if(!__strcmp(*to_tokenize,""))
		return(free(*to_tokenize), 0);
	return (1);
}

int	__treat_user_input(char *arg, t_msh *msh)
{
	t_list		*token;
	t_lexing	*lexing;
	char		*to_tokenize;
	t_lexing	*first_error;
	int			syntax_tree;
	t_lexing	*parenthesis;

	token = NULL;
	lexing = NULL;
	parenthesis = NULL;
	if(!__check_input(arg, &to_tokenize, msh))
		return (0);
	if (!__tokenize(to_tokenize, &token, msh))
		return (free(to_tokenize), __exit_error(msh, 3, "tokenize"));
	free(to_tokenize);	
	if (!__lexing(token, &lexing))
		return (__lexing_full_list_clear(&lexing), __exit_error(msh, 3, "lexing"));
	first_error = __synthax_checker(lexing, msh);
	if(!__handle_here_doc(lexing, first_error, msh))
		return (__lexing_full_list_clear(&lexing), __exit_error(msh, 3, "here_doc"));
	if(msh->syntax_error == 2)
		return (__lexing_full_list_clear(&lexing), -1);
	if(!__give_node(__count_node(lexing), 1))
		return (__lexing_full_list_clear(&lexing),__exit_error(msh, 3, "create tree"));
	syntax_tree = __create_tree(lexing, &(msh->root), &parenthesis);
	DEBUG && print2D(msh->root);
	//printf("clear parenthesis\n");
	__lexing_full_list_clear(&parenthesis);
	parenthesis = NULL;
	if (syntax_tree == 0)
		return (__destroy_tree(&msh->root), -1);
	//__exit_error(msh, 1, "Malloc test");
	msh->rv = __execute_tree(msh->root, msh);
	//printf("destroy tree\n");
	__destroy_tree(&msh->root);
	return (msh->rv);
}

int __non_interative_mode(char **av, t_msh *msh)
{
	int i;
	char **inputs;

	i = 0;
	if (!__strcmp(av[1], "-c"))
	{
		if (av[2])
		{
			inputs = __split_unquoted_charset(av[2], "\n;");
			if (!inputs)
				return(__exit_error(msh, 3, "split input"));
			msh->all_input = inputs;
			while(inputs[i])
			{
				__treat_user_input(inputs[i], msh);
				i++;
			}	
			return  (__exit(msh));
		}
		else
			return(__putendl_fd("Minishell : -c: option requires an argument", 2), __exit_error(msh, 2, ""));
	}
	else
		return(__putendl_fd("Minishell : invalid option", 2), __exit_error(msh, 1, ""));

}

static void __update_rv(t_msh *msh)
{
	if (g_rv > 0)
		msh->rv = g_rv;
	g_rv = 0;
}

int	__interactive_mode(t_msh *msh)
{
	char	*arg;
	char 	**inputs;
	int		i;

	while (42)
	{
		signal(SIGINT, __signal);
		signal(SIGQUIT, __signal);
		arg = readline(__get_prompt(msh));
		//arg = readline(NULL);
		add_history(arg);
		__update_rv(msh);
		if (arg == NULL)
			break ;
		signal(SIGINT, __signal_treat);
		inputs = __split_unquoted_charset(arg, "\n;");
		if (!inputs)
				return(__exit_error(msh, 3, "spliting inputs"));
		msh->all_input = inputs;
		i = -1;
		while(inputs[++i])
			__treat_user_input(inputs[i], msh);
		free_split(inputs);
		msh->all_input = NULL;
		inputs = NULL;
		free(arg);
	}
	return (msh->rv);
}


int	main (int ac, char *av[], char *envp[])
{
	t_msh	msh;
	
	msh = (t_msh){.rv = 0};
	//printf("size = %lu\n", sizeof(t_glob));
	if (!get_env(&msh, envp))
		return (1);
	if (ac > 1)
		__non_interative_mode(av, &msh);
	__interactive_mode(&msh);
	__exit(&msh);
	return (0);
}



	//parcourir l'arbre et expand juste avant d'exe
	// ceci afin de prendre en compte les modif de l'env	
	
//	debut de la fx d'exe de node
	
	/*
	
	__print_lexing(lexing);
	__parameter_expand_token(lexing, msh);
	__hande_wildcards(msh, lexing);
	//__glob_expand_token(lexing);
	cmd = create_cmd_list(lexing, msh);
	if (!cmd)
		return (-1);
	DEBUG && print_cmd_lst(cmd);
	execute_seq(cmd, msh);
	//__cmd_node_list_clear(cmd);
	*/

// fin de la fx d'exe de node
