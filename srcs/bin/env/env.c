/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:17:11 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 15:18:58 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__env(char **env)
{
	int 	i;
	char	*tmp;

	tmp = NULL;
	if (!env)
		return (__FAIL);
	tmp = __strdup("");
	i = 0;
	while (env[i] != NULL)
	{
		tmp = __strjoin(tmp, env[i]);
		tmp = __strjoin(tmp, "\n");
		i++;
	}
	__putstr_fd(tmp, 1);
	free(tmp);
	return (__SUCCESS);
}