/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:41:54 by jremy             #+#    #+#             */
/*   Updated: 2022/04/06 15:16:44 by jremy            ###   ########.fr       */
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

int	__is_only_char(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != c)
			return (0);
		i++;
	}
	return (1);
}

static void	__put_newline(int flags, int fd)
{
	if (!flags)
		__putstr_fd("\n", fd);
}

int	__echo(char **str, int fd)
{
	int	i;
	int	flags;

	if (!str || !str[0])
		return (__FAIL);
	flags = 0;
	i = 1;
	if (__get_flags(str) > 1)
	{
		if (__strncmp(str[i], "-n", 2) == 0
			&& (__is_only_char(str[i] + 2, 'n')))
			flags = 1;
		while (str[i] != NULL && (__strncmp(str[i], "-n", 2) == 0
				&& (__is_only_char(str[i] + 2, 'n'))))
			i++;
	}
	while (str[i] != NULL)
	{
		__putstr_fd(str[i], fd);
		if (str[i + 1])
			__putstr_fd(" ", fd);
		i++;
	}
	return (__put_newline(flags, fd), __SUCCESS);
}
