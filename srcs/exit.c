/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 15:56:34 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__exit(t_msh *msh)
{	
	__destroy_tree(&msh->root);
	destroy_env(msh);
	free(msh->prompt);
	free_split(msh->all_input);
	msh->all_input = NULL;
	__give_node(0, 1, 0);
	exit (msh->rv);
}

void	__error_msg(int error, char *str)
{
	if (error == 3)
		__putstr_fd("minishell : Malloc error in", 2);
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
	__give_node(0, 1, 0);
	exit (msh->rv);
}
