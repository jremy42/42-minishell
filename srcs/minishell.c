/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/21 17:52:19 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	g_rv;

char	*__get_prompt(t_msh *msh)
{
	char	path[PATH_MAX];
	int		i;

	i = 0;
	__bzero(path, PATH_MAX);
	if (msh->prompt)
		free(msh->prompt);
	if (msh->rv == 0)
		msh->prompt = __strdup(BOLDGREEN"➜  "RESET BOLDCYAN);
	else
		msh->prompt = __strdup(BOLDRED"➜  "RESET BOLDCYAN);
	if (getcwd(path, PATH_MAX))
	{
		while (path[i])
			i++;
		while (path[i] != '/' && i >= 0)
			i--;
		i++;
		msh->prompt = __strjoin(msh->prompt, path + i);
		msh->prompt = __strjoin(msh->prompt, RESET BOLDYELLOW "  ~  "RESET);
	}
	return (msh->prompt);
}

static void	get_env(t_msh *msh, char *envp[])
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		i++;
	msh->envp = (char ***)malloc((i + 1) * sizeof(char **));
	msh->envp[i] = NULL;
	while (i--)
	{
		msh->envp[i] = (char **)malloc((3) * sizeof(char *));
		msh->envp[i][0] = __strdup(envp[i]);
		msh->envp[i][1] = __strdup("1");
		msh->envp[i][2] = NULL;
	}
}

int __print_lexing(t_lexing *lexing)
{
	while (lexing)
	{
		fprintf(stderr, "Le token vaut : >%s< et de type = %d\n", lexing->token, lexing->type); 
		lexing = lexing->next;
	}	
	return (1);
}

void print2DUtil(t_node *root, int space)
{
	t_lexing *index;
	
	// Base case
	if (root == NULL)
		return;
 
	// Increase distance between levels
	space += COUNT;
 
	// Process right child first
	print2DUtil(root->right, space);
 
	// Print current node after space
	// count
	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	printf("%d\n", root->kind);
	if (root->kind == SEQUENCE)
	{
		index = root->leaf_lexing;
		while (index)
		{
			printf("[%s] ", index->token);
			index = index->next;
		}
	}
 
	// Process left child
	print2DUtil(root->left, space);
}

int print2D(t_node *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
   printf("\n");
   return (1);
}

int	__treat_user_input(char *arg, t_msh *msh)
{
	t_list		*start;
	t_lexing	*lexing;
	//t_cmd		*cmd;
	char		*to_tokenize;
	t_lexing	*first_error;

	start = NULL;
	lexing = NULL;
	(void)msh;
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
	DEBUG && print2D(msh->root);
	msh->rv = __execute_tree(msh->root, msh);
	
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
	__destroy_tree(&msh->root);
	return (msh->rv);
}

int	main (int ac, char *av[], char *envp[])
{
	char	*arg;
	t_msh	msh;
	(void)	ac;
	(void)	av;
	(void)	envp;

	msh = (t_msh){.rv = 0};
	get_env(&msh, envp);
	if (ac > 1)
	{
		if (!__strcmp(av[1], "-c"))
		{
			if (av[2])
				return (__treat_user_input(av[2], &msh), __exit(&msh));
			else
			{
				msh.rv = 2;
				return(__putendl_fd("Minishell : -c: option requires an argument", 2),__exit(&msh));
			}
		}
		else
		{
			msh.rv = 1;
			return(__putendl_fd("Minishell : invalid option", 2),__exit(&msh));
		}
	}
	while (42)
	{
		signal(SIGINT, __signal);
		signal(SIGQUIT, __signal);
		arg = readline(__get_prompt(&msh));
		add_history(arg);
		if (arg == NULL)
		{
			if (arg)
				free(arg);
			arg = NULL;
			break ;
		}
		signal(SIGINT, __signal_treat);
		__treat_user_input(arg, &msh);
		free(arg);
	}
	__exit(&msh);
}
