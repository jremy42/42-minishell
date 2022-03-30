/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 15:07:24 by jremy            ###   ########.fr       */
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

int	get_env(t_msh *msh, char *envp[], int size)
{
	int	i;

	i = -1;
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
			return (free(msh->envp[i][0]), free(msh->envp[i]),
				partial_destroy_env(msh, i), 0);
		msh->envp[i][2] = NULL;
	}
	if (update_pwd(msh) == __MALLOC || update_shlvl(msh) == __MALLOC)
		__exit_error(msh, 240, "Malloc Error in update pwd or update shlvl");
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

static void __init_user_input_struct(t_user_input *ui)
{
	ui->token = NULL;
	ui->lexing = NULL;
	ui->parenthesis = NULL;
}

int	__treat_user_input(char *arg, t_msh *msh)
{
	t_user_input ui;
	int ret_hd;

	__init_user_input_struct(&ui);
	if(!__check_input(arg, &ui.to_tokenize, msh))
		return (0);
	if (!__tokenize(ui.to_tokenize, &ui.token, msh))
		return (free(ui.to_tokenize), __exit_error(msh, 3, "tokenize"));
	free(ui.to_tokenize);	
	if (!__lexing(ui.token, &ui.lexing))
		return (__lexing_full_list_clear(&ui.lexing), __exit_error(msh, 3, "lexing"));
	ui.first_error = __synthax_checker(ui.lexing, msh);
	ret_hd = __handle_here_doc(ui.lexing, ui.first_error, msh);
	if (!ret_hd)
		return (__lexing_full_list_clear(&ui.lexing), __exit_error(msh, 3, "here_doc"));
	if (ret_hd == 130)
		return (__lexing_full_list_clear(&ui.lexing), 0);
	if(msh->syntax_error == 2)
		return (__lexing_full_list_clear(&ui.lexing), -1);
	if(!__give_node(__count_node(ui.lexing), 1))
		return (__lexing_full_list_clear(&ui.lexing),__exit_error(msh, 3, "create tree"));
	ui.syntax_tree = __create_tree(ui.lexing, &(msh->root), &ui.parenthesis);
	__lexing_full_list_clear(&ui.parenthesis);
	if (ui.syntax_tree == 0)
		return (__destroy_tree(&msh->root), -1);
	msh->rv = __execute_tree(msh->root, msh);
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

void	__clean_inputs(char ** inputs, t_msh *msh, char *arg)
{
	free_split(inputs);
	inputs = NULL;
	msh->all_input = NULL;
	free(arg);
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
		__clean_inputs(inputs, msh, arg);
	}
	return (msh->rv);
}


int	main (int ac, char *av[], char *envp[])
{
	t_msh	msh;
	
	msh = (t_msh){.rv = 0};
	//printf("size = %lu\n", sizeof(t_glob));
	if (!get_env(&msh, envp, get_size_env(envp)))
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
