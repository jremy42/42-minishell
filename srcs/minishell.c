/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/11 16:59:22 by jremy            ###   ########.fr       */
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
	if (g_rv == 0)
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

void __print_lexing(t_lexing *lexing)
{
	while (lexing)
	{
		printf("Le token vaut : >%s< et de type = %d\n", lexing->token, lexing->type); 
		lexing = lexing->next;
	}	
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
		index = root->tmp;
		while (index)
		{
			printf("[%s] ", index->token);
			index = index->next;
		}
	}
 
    // Process left child
    print2DUtil(root->left, space);
}

void print2D(t_node *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
   printf("\n");
}

int	__mini_parsing(char *arg, t_msh *msh)
{
	t_list		*start;
	t_lexing	*lexing;
	t_cmd		*cmd;
	char		*to_tokenize;
	t_lexing	*first_error;

	start = NULL;
	lexing = NULL;
	(void)msh;
	to_tokenize = __strtrim(arg, " \f\t\r\v");
	__tokenize(to_tokenize, &start);
	free(to_tokenize);
	if (__lexing(start, &lexing) < 0)
		return (write(2, "Malloc error\n", 14), -1);
	//__print_lexing(lexing);
	first_error = __synthax_checker(lexing);
	if (first_error)
		fprintf(stderr, "DEBUG first error : %s\n", first_error->token);
	__handle_here_doc(lexing, first_error, msh);
	if(first_error)
		return (-1);
	__print_lexing(lexing);
	if (!__create_tree(lexing, &(msh->root)))
		return (-1);
	print2D(msh->root);
	//free lexing a faire a la fin de l'ex
	//cmd = miniparsing(msh->root->tmp);
	cmd = create_cmd_list(lexing, msh);
	if (!cmd)
		return (-1);
	print_cmd_lst(cmd);
	execute_seq(cmd, msh);
	//__cmd_list_clear(cmd);
	return (0);
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
	while (42)
	{
		signal(SIGINT, __signal);
		signal(SIGQUIT, __signal);
		arg = readline(__get_prompt(&msh));
		add_history(arg);
		if (__strncmp(arg, "exit", __strlen("exit")) == 0 || arg == NULL)
		{
			if (arg)
				free(arg);
			arg = NULL;
			break ;
		}
		__mini_parsing(arg, &msh);
		free(arg);
	}
	__exit(&msh);
}

/*
	__pwd(1);
	__echo(arg, 1);
	__cd("srcs");
	__pwd(1);
	__cd("/dev");
	__pwd(1);
	__env(NULL);
	*/


/*

	argv = __split(arg, ' ');
	if (__strncmp(argv[0], "echo", 4) == 0)
		__echo(argv, 1);
	if (__strncmp(argv[0], "cd", 2) == 0)
		__cd(argv[1], msh);
	if (__strncmp(argv[0], "pwd", 3) == 0)
		__pwd(1);
	if (__strncmp(argv[0], "env", 3) == 0)
		__env(msh);
	if (__strncmp(argv[0], "export", 6) == 0)
		__export(argv + 1, msh);
	if (__strncmp(argv[0], "unset", 5) == 0)
		__unset(argv + 1, msh);
	i = -1;
	while (argv[++i])
		free(argv[i]);
	free(argv);

	*/
/*
	index = lexing;
	while (index)
	{
		printf("Le token vaut : >%s< et de type = %d\n", index->token, index->type); 
		index = index->next;
	}	
	*/
