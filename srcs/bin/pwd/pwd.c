/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 16:43:02 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 17:16:11 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__pwd(int fd)
{
	char	path[PATH_MAX];

	if (getcwd(path, PATH_MAX))
	{
		__putstr_fd(path, fd);
		__putstr_fd("\n", fd);
		return (__SUCCESS);
	}
	return (print_error("pwd", NULL, strerror(errno)), __FAIL);
}
