/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/22 11:23:28by jremy            ###   ########.fr       */
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
		return(__putendl_fd("Minishell : Malloc Error", 2), __exit_error(msh, 3), NULL);
	if (getcwd(path, PATH_MAX))
	{
		msh->prompt = __strjoin(msh->prompt, path + __get_pos_last_dir(path));
		if (!msh->prompt)
			return(__putendl_fd("Minishell : Malloc Error", 2), __exit_error(msh, 3), NULL);
		msh->prompt = __strjoin(msh->prompt, RESET BOLDYELLOW "  ~  "RESET);
		if (!msh->prompt)
			return(__putendl_fd("Minishell : Malloc Error", 2), __exit_error(msh, 3), NULL);
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

static int	get_env(t_msh *msh, char *envp[])
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
			return (0);
		msh->envp[i][0] = __strdup(envp[i]);
		if(!msh->envp[i][0])
			return (0);
		msh->envp[i][1] = __strdup("1");
		if(!msh->envp[i][0])
			return (0);
		msh->envp[i][2] = NULL;
	}
	return (1);
}

int	__treat_user_input(char *arg, t_msh *msh)
{
	t_list		*start;
	t_lexing	*lexing;
	char		*to_tokenize;
	t_lexing	*first_error;

	start = NULL;
	lexing = NULL;
	if(!__strcmp(arg,""))
		return(0);
	msh->syntax_error = 0;
	to_tokenize = __strtrim(arg, " \f\t\r\v");
	if(!__strcmp(to_tokenize,""))
		return(free(to_tokenize), 0);
	if (!to_tokenize)
		return (write(2, "Malloc error\n", 14), -1);
	if (!__tokenize(to_tokenize, &start, msh))
		return (free(to_tokenize), __lstclear(&start, free), 0);
	free(to_tokenize);
	if (__lexing(start, &lexing) < 0)
		return (write(2, "Malloc error\n", 14), -1);
	first_error = __synthax_checker(lexing, msh);
	__handle_here_doc(lexing, first_error, msh);
	if(msh->syntax_error == 2)
		return (__lexing_full_list_clear(lexing), -1);
	if (!__create_tree(lexing, &(msh->root)))
		return (__destroy_tree(&msh->root), -1);
	msh->rv = __execute_tree(msh->root, msh);	
	__destroy_tree(&msh->root);
	return (msh->rv);
}
/*
int	__treat_user_input(char *arg, t_msh *msh)
{
	t_list		*start;
	t_lexing	*lexing;
	char		*to_tokenize;
	t_lexing	*first_error;

	start = NULL;
	lexing = NULL;
	if(!__strcmp(arg,""))
		return(0);
	to_tokenize = __strtrim(arg, " \f\t\r\v");
	__tokenize(to_tokenize, &start);
	free(to_tokenize);
	if (__lexing(start, &lexing) < 0)
		return (write(2, "Malloc error\n", 14), -1);
	first_error = __synthax_checker(lexing, msh);
	if (first_error)
		DEBUG && fprintf(stderr, "DEBUG first error : %s\n", first_error->token);
	__handle_here_doc(lexing, first_error, msh);
	DEBUG && __print_lexing(lexing);
	if(first_error)
		return (__lexing_full_list_clear(lexing), -1);
	if (!__create_tree(lexing, &(msh->root)))
		return (__destroy_tree(&msh->root), -1);
	print2D(msh->root);
	msh->rv = __execute_tree(msh->root, msh);	
	__destroy_tree(&msh->root);
	return (msh->rv);
}
*/

static int __non_interative_mode(char **av, t_msh *msh)
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
				return(__putendl_fd("Minishell : Malloc Error", 2), __exit_error(msh, 3));
			while(inputs[i])
			{
				__treat_user_input(inputs[i], msh);
				i++;
			}	
			return (free_split(inputs), __exit(msh));
		}
		else
			return(__putendl_fd("Minishell : -c: option requires an argument", 2), __exit_error(msh, 2));
	}
	else
		return(__putendl_fd("Minishell : invalid option", 2), __exit_error(msh, 1));

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
		add_history(arg);
		__update_rv(msh);
		if (arg == NULL)
			break ;
		signal(SIGINT, __signal_treat);
		inputs = __split_unquoted_charset(arg, "\n;");
		if (!inputs)
				return(__putendl_fd("Minishell : Malloc Error while splitting input", 2), __exit_error(msh, 3));
		i = -1;
		while(inputs[++i])
			__treat_user_input(inputs[i], msh);
		free_split(inputs);
		free(arg);
	}
	return (msh->rv);
}


int	main (int ac, char *av[], char *envp[])
{
	t_msh	msh;
	
	msh = (t_msh){.rv = 0};
	if (!get_env(&msh, envp))
		return (destroy_env(&msh), 1);
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