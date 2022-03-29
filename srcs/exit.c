/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/29 10:45:23 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void __destroy_tree(t_node **current_node)
{
	if (!current_node || !*current_node)
		return ;
    __destroy_tree(&((*current_node)->left));
    __destroy_tree(&((*current_node)->right));
	if ((*current_node)->kind == SEQUENCE)
		__lexing_full_list_clear(&((*current_node)->leaf_lexing));
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

int	__exit(t_msh *msh)
{	
	__destroy_tree(&msh->root);
	destroy_env(msh);
	free(msh->prompt);
	free_split(msh->all_input);
	msh->all_input = NULL;
	__give_node(0, 1);
	exit (msh->rv);
}

void __error_msg(int error, char *str)
{
	if (error == 3)
		__putstr_fd("Minishell : Malloc error in",2);
	__putstr_fd(str, 2);
}

int	__exit_error(t_msh *msh, int error, char *str)
{	
	msh->rv = error;
	__error_msg(msh->rv, str);
	__destroy_tree(&msh->root);
	destroy_env(msh);
	free(msh->prompt);
	free_split(msh->all_input);
	msh->all_input = NULL;
	__give_node(0, 1);
	exit (msh->rv);
}
