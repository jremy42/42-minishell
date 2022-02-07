/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:41:54 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 19:09:22 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	int __get_flags(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	__echo(char **str, int fd)
{
	int	i;
	int	flags;

	flags = 0;
	i = 0;
	i++;
	if (__get_flags(str) > 0)
	{
		if (__strncmp(str[i], "-n", 2) == 0)
			flags = 1;
	}
	while (__strncmp(str[i], "-n", 2) == 0)
		i++;
	while (str[i])
	{
		__putstr_fd(str[i], fd);
		if (str[i + 1])
			__putstr_fd(" ",fd);
		i++;
	}
	if (flags == 0)
		__putstr_fd("\n", fd);
	return (__SUCCESS);
}