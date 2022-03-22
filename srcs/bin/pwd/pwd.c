/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 16:43:02 by jremy             #+#    #+#             */
/*   Updated: 2022/03/22 09:11:02 by fle-blay         ###   ########.fr       */
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
	return (__FAIL);
}
