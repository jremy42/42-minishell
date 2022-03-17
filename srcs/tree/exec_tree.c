/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 09:50:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/17 11:27:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
static int __execute_pipe_seq(t_lexing *lexing, t_msh *msh)
{
	t_cmd *cmd;
	
	DEBUG && __print_lexing(lexing);
	if (!__parameter_expand_token(lexing, msh))
		__exit(msh);
	__handle_wildcards(msh, lexing);
	cmd = create_cmd_list(lexing, msh);
	if (!cmd)
		return (-1);
	DEBUG && print_cmd_lst(cmd);
	execute_seq(cmd, msh);
	DEBUG && fprintf(stderr, "Return code for current pipesequence [%d]\n", msh->rv);
	return (1);
}

int __execute_tree(t_node *current_node, t_msh *msh)
{
	if(current_node->kind == SEQUENCE)
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