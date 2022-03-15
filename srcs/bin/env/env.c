/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:17:11 by jremy             #+#    #+#             */
/*   Updated: 2022/03/15 09:15:38 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__env(t_msh *msh)
{
	int		i;
	char	*tmp;

	tmp = NULL;
	if (!msh->envp)
		return (__FAIL);
	tmp = __strdup("");
	if (!tmp)
		return (__MALLOC);
	i = 0;
	while (msh->envp[i])
	{
		if (__strncmp(msh->envp[i][1], "1", 2) == 0)
		{
			tmp = __strjoin(tmp, msh->envp[i][0]);
			tmp = __strjoin(tmp, "\n");
			if (!tmp)
				return (__MALLOC);
		}
		i++;
	}
	__putstr_fd(tmp, 1);
	free(tmp);
	return (__SUCCESS);
}