/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:41:54 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 15:26:18 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int	__get_flags(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
		i++;
	return (i);
}

int	__echo(char **str, int fd)
{
	int	i;
	int	flags;

	if(!str || !str[0])
		return (__FAIL);
	flags = 0;
	i = 0;
	i++;
	if (__get_flags(str) > 1)
	{
		if (__strncmp(str[i], "-n", 2) == 0
			&& (__strlen(str[i]) == __strlen("-n")))
			flags = 1;
		while (str[i] != NULL && (__strncmp(str[i], "-n", 2) == 0
			&& (__strlen(str[i]) == __strlen("-n"))))
			i++;
	}
	while (str[i] != NULL)
	{
		__putstr_fd(str[i], fd);
		if (str[i + 1])
			__putstr_fd(" ", fd);
		i++;
	}
	if (flags == 0)
		__putstr_fd("\n", fd);
	return (__SUCCESS);
}
