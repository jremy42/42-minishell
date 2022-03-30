/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/16 09:50:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 15:36:48 by jremy            ###   ########.fr       */
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
		return (__putendl_fd("Empty parenthesis", 2), 0);
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



int __execute_pipe_seq(t_lexing *lexing, t_msh *msh)
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
		__exit_error(msh,240, "Malloc error in parameter expand\n");
	DEBUG && printf("Param expand done\n");
	if(!__field_spliting_token(lexing, msh))
		__exit_error(msh,240, "Malloc error in field spliting\n");
	DEBUG && printf("fieldsplit done\n");
	if(!__handle_wildcards(msh, lexing))
		__exit_error(msh,240, "Malloc error in wildcard\n");
	DEBUG && printf("wildcard done\n");
	if (!__quote_removal_token(lexing))
		__exit_error(msh,240, "Malloc error in quote removale\n");
	DEBUG && printf("quote remove done\n");
	cmd = create_cmd_list(lexing, msh);
	if (!cmd)
		__exit_error(msh,240, "Malloc error in create cmd list\n");
	DEBUG && printf("cmd list create done\n");
	print_cmd_lst(cmd);
	execute_seq(cmd, msh);
	DEBUG && printf("exit list done\n");
	if(msh->rv == 240)
		__exit_error(msh,240, "Malloc error in exe\n");
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