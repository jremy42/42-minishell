/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 18:13:58 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>

int	g_rv;

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
		return (__exit_error(msh, 3, "get prompt"), NULL);
	if (getcwd(path, PATH_MAX))
	{
		msh->prompt = __strjoin2(msh->prompt,
				path + __get_pos_last_dir(path), RESET BOLDYELLOW "  ~  "RESET);
		if (!msh->prompt)
			return (__exit_error(msh, 3, "get prompt"), NULL);
	}
	else
	{
		msh->prompt = __strjoin(msh->prompt, RESET);
		if (!msh->prompt)
			return (__exit_error(msh, 3, "get prompt"), NULL);
	}
	return (msh->prompt);
}

int	__treat_user_input(char *arg, t_msh *msh, t_user_input *ui)
{
	if (!__check_input(arg, &ui->to_tokenize, msh))
		return (0);
	ui->ret_tokenize = __tokenize(ui->to_tokenize, &ui->token, msh);
	if (ui->ret_tokenize == __MALLOC_TOKEN)
		return (free(ui->to_tokenize), __exit_error(msh, 3, "tokenize\n"));
	if (ui->ret_tokenize == __SYNTAX_ERROR)
		return (free(ui->to_tokenize), 0);
	free(ui->to_tokenize);
	if (!__lexing(ui->token, &ui->lexing))
		return (__lex_fclear(&ui->lexing),
			__exit_error(msh, 3, "lexing\n"));
	ui->first_error = __synthax_checker(ui->lexing, msh);
	if (here_doc_handler(ui, msh) == 0)
		return (0);
	if (msh->syntax_error == 2)
		return (__lex_fclear(&ui->lexing), -1);
	msh->node_max = __count_node(ui->lexing);
	if (!__give_node(msh->node_max, 1, 0))
		return (__lex_fclear(&ui->lexing), __exit_error(msh, 3, "create tree\n"));
	ui->syntax_tree = __create_tree(ui->lexing, &(msh->root), &ui->parenthesis);
	__lex_fclear(&ui->parenthesis);
	if (ui->syntax_tree == 0)
		return (__destroy_tree(&msh->root), -1);
	msh->rv = __execute_tree(msh->root, msh);
	return (__destroy_tree(&msh->root), msh->rv);
}

int	__non_interative_mode(char **av, t_msh *msh, t_user_input *ui)
{
	int		i;
	char	**inpts;

	i = -1;
	if (!__strcmp(av[1], "-c"))
	{
		if (av[2])
		{
			inpts = __split_unquoted_charset(av[2], "\n;");
			if (!inpts)
				return (__exit_error(msh, 3, "split input"));
			msh->all_input = inpts;
			while (inpts[++i])
				__treat_user_input(inpts[i], msh, __init_user_input_struct(ui));
			return (__exit(msh));
		}
		else
			return (__putendl_fd("minishell: -c: option requires an\
					argument", 2), __exit_error(msh, 2, ""));
	}
	else
		return (__putendl_fd("minishell: invalid option", 2),
			__exit_error(msh, 1, ""));
}

int	__interactive_mode(t_msh *msh, t_user_input *ui)
{
	char	*arg;
	char	**inputs;
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
			return (__exit_error(msh, 3, "spliting inputs"));
		msh->all_input = inputs;
		i = -1;
		while (inputs[++i])
			__treat_user_input(inputs[i], msh, __init_user_input_struct(ui));
		__clean_inputs(inputs, msh, arg);
	}
	return (msh->rv);
}

int	main(int ac, char *av[], char *envp[])
{
	t_msh			msh;
	t_user_input	ui;

	msh = (t_msh){.rv = 0};
	if (!get_env(&msh, envp, get_size_env(envp)))
		return (1);
	if (ac > 1)
		__non_interative_mode(av, &msh, &ui);
	__interactive_mode(&msh, &ui);
	__exit(&msh);
	return (0);
}
