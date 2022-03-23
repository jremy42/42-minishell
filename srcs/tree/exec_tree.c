/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 09:50:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/22 15:19:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int __check_parenthesis_in_pipe(t_lexing *lexing)
{
	int			p_left;
	int			p_right;

	p_left = 0;
	p_right = 0;
	while(lexing && lexing->type == P_LEFT)
	{
		p_left++;
		lexing = lexing->next;
	}
	if (lexing->type == P_RIGHT)
		return (fprintf(stderr, "Empty parenthesis\n"), 0);
	while(lexing && lexing->type != PIPE)
	{
		if(lexing->type == P_RIGHT)
			p_right++;
		if (lexing->type == P_LEFT)
			return (0);
		lexing = lexing->next;
	}
	if (p_left != p_right)
		return (0);
	return (1);
}



static int __execute_pipe_seq(t_lexing *lexing, t_msh *msh)
{
	t_cmd *cmd;
	int	trim;
	t_lexing *next_pipe;
	
	DEBUG && __print_lexing(lexing);
	trim = 1;
	next_pipe = lexing;
	while(next_pipe)
	{ 
		if(!__check_parenthesis_in_pipe(next_pipe))
		{
			DEBUG && printf("synthax error with parenthesis in %s:\n", next_pipe->token);
			return (-1);
		}
		while(next_pipe->next && next_pipe->type != PIPE)
			next_pipe = next_pipe->next;
		trim = 1;
		next_pipe = next_pipe->next;
	}
	if (!__parameter_expand_token(lexing, msh))
		__exit(msh);
	__field_spliting_token(lexing, msh);
	__handle_wildcards(msh, lexing);
	__quote_removal_token(lexing, msh);
	cmd = create_cmd_list(lexing, msh);
	if (!cmd)
		return (-1);
	print_cmd_lst(cmd);
	execute_seq(cmd, msh);
	return (msh->rv);
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