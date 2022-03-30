/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_childs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:21:22 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 10:55:34 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static char	*__create_path_and_cmd(char *path, char *cmd)
{
	char	*ret;
	int		size;
	int		i;

	i = 0;
	if (!path || !cmd)
		return (NULL);
	size = __strlen(path);
	size += __strlen("/");
	size += __strlen(cmd);
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

static char	*__get_path(char **path, char *cmd_n)
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

void	__exit_child(t_sequ *seq, t_cmd *cmd, int errno_copy, int error)
{
	t_msh	*tmp;

	if (errno_copy)
		cmd->msh->rv = errno_copy;
	if (error)
	{
		cmd->msh->rv = errno_copy;
		__putendl_fd(strerror(errno_copy), 2);
	}
	__clean_tmp_hd(cmd);
	free_split(seq->path);
	free(seq->envp);
	tmp = cmd->msh;
	if (tmp->root)
		__cmd_node_list_clear(cmd);
	else
		__cmd_full_list_clear(cmd);
	__exit(tmp);
}

static void __check_access_and_exit(char *arg, t_sequ *seq, t_cmd *first_cmd)
{
	struct stat	buff;

	if (access(arg, F_OK) < 0)
	{
		print_error(arg, "command not found\n", NULL);
		__exit_child(seq, first_cmd, 127, 0);
	}
	if (access(arg, X_OK) < 0)
	{
		print_error(arg, "Permission denied\n", NULL);
		__exit_child(seq, first_cmd, 126, 0);
	}
	stat(arg, &buff);
	if (S_ISDIR(buff.st_mode))
	{
		print_error(arg, "Is a directory\n", NULL);
		__exit_child(seq, first_cmd, 126, 0);
	}
	print_error(arg, "command not found\n", NULL);
	__exit_child(seq, first_cmd, 127, 0);
}

static void	__check_access_and_execve(char *path_cmd, char **arg, t_sequ *seq, t_cmd *first_cmd)
{
	struct stat	buff;

	if (access(path_cmd, X_OK) < 0)
	{
		print_error(arg[0], "Permission denied\n", NULL);
		__exit_child(seq, first_cmd, 126, 0);
	}
	stat(path_cmd, &buff);
	if (S_ISDIR(buff.st_mode))
	{
		print_error(arg[0], "Is a directory\n", NULL);
		__exit_child(seq, first_cmd, 126, 0);
	}
	execve(path_cmd, arg, seq->envp);
}

void	execute_child(t_sequ *seq, t_cmd *cmd, t_cmd *first_cmd)
{
	char		*path_cmd;

	if (cmd->redirect)
		__handle_redirect(seq, cmd);
	if (!cmd->arg[0])
		__exit_child(seq, first_cmd, 0, 0);
	if (__is_builtin(cmd->arg))
	{
		__exec_builtin(cmd->arg, cmd->msh, cmd);
		__exit_child(seq, first_cmd, cmd->msh->rv, 0);
	}
	path_cmd = __get_path(seq->path, cmd->arg[0]);
	if (!path_cmd)
		__check_access_and_exit(cmd->arg[0], seq, first_cmd);
	if (path_cmd)
		__check_access_and_execve(path_cmd, cmd->arg, seq, first_cmd);
	__exit_child(seq, first_cmd, errno, 0);
}
