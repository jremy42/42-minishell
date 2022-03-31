/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:58:20 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 18:11:47 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_error(char *cmd, char *arg, char *error_msg)
{
	__putstr_fd("minishell: ", 2);
	__putstr_fd(cmd, 2);
	__putstr_fd(": ", 2);
	__putstr_fd(arg, 2);
	if (error_msg)
	{
		__putstr_fd(": ", 2);
		__putendl_fd(error_msg, 2);
	}
	else
		__putstr_fd("\n", 2);
	return (1);
}

void	__destroy_tree(t_node **current_node)
{
	if (!current_node || !*current_node)
		return ;
	__destroy_tree(&((*current_node)->left));
	__destroy_tree(&((*current_node)->right));
	if ((*current_node)->kind == SEQUENCE)
		__lex_fclear(&((*current_node)->leaf_lexing));
	else
		free((*current_node)->leaf_lexing);
	free(*current_node);
	*current_node = NULL;
}

void	destroy_env(t_msh *msh)
{
	int	i;

	i = 0;
	if (!msh->envp)
		return ;
	while (msh->envp[i])
	{
		free(msh->envp[i][0]);
		free(msh->envp[i][1]);
		free(msh->envp[i]);
		i++;
	}
	free(msh->envp);
}
