/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 12:56:15 by jremy             #+#    #+#             */
/*   Updated: 2022/04/08 10:32:55 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__catch_error_lex_modifier(int (*p)(), t_lexing **lexing, t_msh *msh)
{
	int	ret;

	ret = p(lexing);
	if (ret == 0)
		__exit_error(msh, 240, "Malloc error in wildcard\n");
	if (ret == -1)
	{
		__putendl_fd("minishell: ambiguous redirect", 2);
		msh->rv = 1;
		return (0);
	}
	return (1);
}

int	__catch_error(int (*p)(), t_lexing *lexing, t_msh *msh)
{
	int	ret;

	ret = p(lexing);
	if (ret == 0)
		__exit_error(msh, 240, "Malloc error in wildcard\n");
	if (ret == -1)
	{
		__putendl_fd("minishell: ambiguous redirect", 2);
		msh->rv = 1;
		return (0);
	}
	return (1);
}
