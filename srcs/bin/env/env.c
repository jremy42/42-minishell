/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:17:11 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 10:19:44 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__env(t_msh *msh)
{
	int		i;

	if (!msh->envp)
		return (__SUCCESS);
	i = 0;
	while (msh->envp[i])
	{	
		if (!__strcmp(msh->envp[i][1], "1") && __strchr(msh->envp[i][0], '=' ))
			__putendl_fd(msh->envp[i][0], 1);
		i++;
	}
	return (__SUCCESS);
}
