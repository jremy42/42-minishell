/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 09:50:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 16:03:14 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__find_operator_in_pipeseq(t_lexing *lexing)
{
	while (lexing)
	{
		if (lexing->type == OPERATOR)
			return (1);
		lexing = lexing->next;
	}
	return (0);
}

int	__check_parenthesis_in_pipe(t_lexing *lexing)
{
	int	p_left;
	int	p_right;

	p_left = 0;
	p_right = 0;
	while (lexing && lexing->type == P_LEFT)
	{
		p_left++;
		lexing = lexing->next;
	}
	if (lexing->type == P_RIGHT)
		return (__putendl_fd("Empty parenthesis", 2), 0);
	while (lexing && lexing->type != PIPE)
	{
		if (lexing->type == P_RIGHT)
			p_right++;
		if (lexing->type == P_LEFT)
			return (0);
		lexing = lexing->next;
	}
	if (p_left != p_right)
		return (0);
	return (1);
}

int	__create_cmd_and_exe(t_lexing *lexing, t_msh *msh)
{
	t_cmd	*cmd;

	cmd = NULL;
	if (!__parameter_expand_token(lexing, msh))
		__exit_error(msh, 240, "Malloc error in parameter expand\n");
	if (!__field_spliting_token(lexing, msh))
		__exit_error(msh, 240, "Malloc error in field spliting\n");
	if (!__handle_wildcards(lexing))
		__exit_error(msh, 240, "Malloc error in wildcard\n");
	if (!__quote_removal_token(lexing))
		__exit_error(msh, 240, "Malloc error in quote removale\n");
	cmd = create_cmd_list(lexing, msh);
	if (!cmd)
		__exit_error(msh, 240, "Malloc error in create cmd list\n");
	execute_seq(cmd, msh);
	if (msh->rv == 240)
		__exit_error(msh, 240, "Malloc error in exe\n");
	return (msh->rv);
}

int	__execute_pipe_seq(t_lexing *lexing, t_msh *msh)
{
	t_lexing	*next_pipe;

	if (__find_operator_in_pipeseq(lexing))
	{
		__putstr_fd("minishell: syntax error : operator within pipeseq\n", 2);
		msh->rv = 2;
		__give_node(msh->node_max, 0, 1);
		return (-1);
	}
	next_pipe = lexing;
	while (next_pipe)
	{
		if (!__check_parenthesis_in_pipe(next_pipe))
		{
			__putstr_fd("minishell: synthax error with parenthesis in:", 2);
			__putendl_fd(next_pipe->token, 2);
			msh->rv = 2;
			return (-1);
		}
		while (next_pipe->next && next_pipe->type != PIPE)
			next_pipe = next_pipe->next;
		next_pipe = next_pipe->next;
	}
	return (__create_cmd_and_exe(lexing, msh));
}

int	__execute_tree(t_node *current_node, t_msh *msh)
{
	if (current_node->kind == SEQUENCE)
	{
		__execute_pipe_seq(current_node->leaf_lexing, msh);
		return (msh->rv);
	}
	else if (current_node->kind == AND)
	{
		if (__execute_tree(current_node->left, msh) == 0)
			__execute_tree(current_node->right, msh);
		return (msh->rv);
	}
	else if (current_node->kind == OR)
	{
		if (__execute_tree(current_node->left, msh) == 0)
			return (msh->rv);
		__execute_tree(current_node->right, msh);
		return (msh->rv);
	}
	return (0);
}
