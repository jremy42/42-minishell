/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:13:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/30 10:19:32 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	__access_dir(char *dir, char *true_dir)
{
	struct stat	buff;

	if (access(dir, F_OK) < 0 || access(dir, X_OK) < 0)
		return (print_error("cd", true_dir, strerror(errno)), 0);
	stat(dir, &buff);
	if (!S_ISDIR(buff.st_mode))
	{
		__putstr_fd("minishell : cd : ", 2);
		__putstr_fd(dir, 2);
		__putendl_fd("Not a directory", 2);
		return (0);
	}
	return (1);
}

static int	__check_home(t_msh *msh)
{
	if (!get_key(msh, "HOME") || !__access_dir(get_key(msh, "HOME"),
			get_key(msh, "HOME")))
		return (print_error("cd", "HOME not set\n", NULL), __FAIL);
	if (update_oldpwd(msh) == __MALLOC)
		return (__MALLOC);
	chdir(get_key(msh, "HOME"));
	return (update_pwd(msh));
}

int	__cd(char *new_path, t_msh *msh)
{
	if (!new_path)
		return (__check_home(msh));
	else if (!__strcmp(new_path, "-"))
		return (chdir_previous(msh));
	else
	{
		if (new_path[0] == '/')
		{
			if (!__access_dir(new_path, new_path))
				return (__FAIL);
			if (update_oldpwd(msh) == __MALLOC)
				return (__MALLOC);
			chdir(new_path);
			return (update_pwd(msh));
		}
		else
			return (chdir_absolute_path(new_path, msh));
	}
	return (__FAIL);
}
