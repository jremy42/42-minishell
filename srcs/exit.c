/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/22 09:42:18 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__destroy_tree(t_node *current_node)
{
	if (!current_node)
		return ;
	__destroy_tree(current_node->left);
	__destroy_tree(current_node->right);
	if (current_node->kind == SEQUENCE)
		__lexing_full_list_clear(current_node->leaf_lexing);
	else
		free(current_node->leaf_lexing);
	free(current_node);
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

int	__exit(t_msh *msh)
{	
	destroy_env(msh);
	free(msh->prompt);
	exit (msh->rv);
}
