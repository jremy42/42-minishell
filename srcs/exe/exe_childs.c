/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_childs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:21:22 by jremy             #+#    #+#             */
/*   Updated: 2022/03/11 11:29:26 by jremy            ###   ########.fr       */
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
	size = __strlen(path) + __strlen("/") + __strlen(cmd);
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
	while (path[i])
	{
		*path_cmd = __create_path_and_cmd(path[i], cmd_name);
		if (!*path_cmd)
			return(__putstr_fd("MALLOC ERROR\n", 2), 0);
		if (access(*path_cmd, F_OK) == 0)
			return (1);
		i++;
		free(*path_cmd);
	}
	return (0);
}

static char *__get_path(char **path, char *cmd_name)
{
    int		i;
    char	*path_cmd;

    i = 0;
	if (__strchr(cmd_name, '/') != NULL)
	{
		if (__strncmp(cmd_name, "./", 2) == 0 && access(cmd_name + 2, F_OK) == 0)
				return (cmd_name + 2);
		if (access(cmd_name, F_OK) == 0)
			return (cmd_name);
		if (cmd_name[__strlen(cmd_name) - 1] == '/')
            return (NULL);
	}
	if (path == NULL)
		return (NULL);
	else
    {
		if (__try_paths(&path_cmd, path, cmd_name) == 1)
			return (path_cmd);
    }
    return(__putstr_fd("Command not found\n", 2), NULL);
}

void __exit_child(t_sequ *seq, t_cmd *cmd, int errno_copy, int error)
{
	t_msh *tmp;

	if (error)
	{
		cmd->msh->rv = errno_copy;
		__putstr_fd(strerror(errno_copy), 2);
	}
	free_split(seq->path);
	free(seq->envp);
	tmp = cmd->msh;
	__cmd_list_clear(cmd);
    __exit(tmp);
}

void execute_child(t_sequ *seq, t_cmd *cmd)
{
    char *path_cmd;

	fprintf(stderr, "cmd->redirect =>%p\n", cmd->redirect);	
	if(cmd->redirect)
		__handle_redirect(seq, cmd);
	if (!cmd->arg[0])
		__exit_child(seq, cmd, 0, 0);
	if (__is_builtin(cmd->arg))
	{
		__exec_builtin(cmd->arg, cmd->msh);
		__exit_child(seq, cmd, 0, 0);
	}
    path_cmd = __get_path(seq->path, cmd->arg[0]);
    execve(path_cmd, cmd->arg, seq->envp);
    exit (0);
}