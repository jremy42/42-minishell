/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_childs_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 18:15:22 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 18:27:19 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static int	__get_path_size(char *path, char *cmd)
{
	int	size;

	size = __strlen(path);
	size += __strlen("/");
	size += __strlen(cmd);
	return (size);
}

static char	*__create_path_and_cmd(char *path, char *cmd)
{
	char	*ret;
	int		size;
	int		i;

	i = 0;
	if (!path || !cmd)
		return (NULL);
	size = __get_path_size(path, cmd);
	ret = (char *)malloc((size + 1) * sizeof(char));
	if (!ret)
		return (__putstr_fd("Malloc Error", 2), NULL);
	ret[size] = '\0';
	while (path[i])
	{
		ret[i] = path[i];
		i++;
	}
	ret[i] = '/';
	i = 0;
	while (cmd[i])
	{
		ret[i + __strlen(path) + __strlen("/")] = cmd[i];
		i++;
	}
	return (ret);
}

static int	__try_paths(char **path_cmd, char **path, char *cmd_name)
{
	int	i;

	i = 0;
	if (!path)
		return (0);
	while (path[i])
	{
		*path_cmd = __create_path_and_cmd(path[i], cmd_name);
		if (!*path_cmd)
			return (__putstr_fd("MALLOC ERROR\n", 2), 0);
		if (access(*path_cmd, F_OK) == 0)
			return (1);
		i++;
		free(*path_cmd);
	}
	return (0);
}

char	*__get_path(char **path, char *cmd_n)
{
	int		i;
	char	*path_cmd;

	i = 0;
	if (__strchr(cmd_n, '/') != NULL)
	{
		if (__strncmp(cmd_n, "./", 2) == 0 && access(cmd_n + 2, F_OK) == 0)
			return (cmd_n + 2);
		if (access(cmd_n, F_OK) == 0)
			return (cmd_n);
		if (cmd_n[__strlen(cmd_n) - 1] == '/')
			return (NULL);
	}
	if (path == NULL)
		return (NULL);
	else
	{
		if (__try_paths(&path_cmd, path, cmd_n) == 1)
			return (path_cmd);
	}
	return (NULL);
}
