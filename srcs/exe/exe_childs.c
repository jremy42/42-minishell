/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_childs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:21:22 by jremy             #+#    #+#             */
/*   Updated: 2022/04/12 18:51:20 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__exit_child(t_sequ *seq, t_cmd *cmd, int errno_copy, int error)
{
	t_msh	*tmp;

	if (errno_copy)
		cmd->msh->rv = errno_copy;
	if (error)
	{
		cmd->msh->rv = error;
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

static void	__check_access_and_exit(char *arg, t_sequ *seq, t_cmd *first_cmd)
{
	struct stat	buff;

	__memset(&buff, 0, sizeof(buff));
	stat(arg, &buff);
	if (__strchr(arg, '/') && S_ISDIR(buff.st_mode))
	{
		print_error(arg, "Is a directory", NULL);
		__exit_child(seq, first_cmd, 126, 0);
	}
	if (__strchr(arg, '/') && !S_ISDIR(buff.st_mode))
	{
		print_error(arg, "Not a directory", NULL);
		__exit_child(seq, first_cmd, 126, 0);
	}
	if (access(arg, F_OK) < 0 || access(arg, X_OK) < 0)
	{
		if (seq->path)
			print_error(arg, "command not found", NULL);
		else
			print_error(arg, "No such file or directory", NULL);
		__exit_child(seq, first_cmd, 127, 0);
	}
	print_error(arg, "command not found", NULL);
	__exit_child(seq, first_cmd, 127, 0);
}

void	__check_access_exe(char *p_cmd, char **arg, t_sequ *s, t_cmd *f_cmd)
{
	struct stat	buff;

	if (access(p_cmd, X_OK) < 0)
	{
		print_error(arg[0], "Permission denied", NULL);
		__exit_child(s, f_cmd, 126, 0);
	}
	stat(p_cmd, &buff);
	if (S_ISDIR(buff.st_mode))
	{
		print_error(arg[0], "Is a directory", NULL);
		__exit_child(s, f_cmd, 126, 0);
	}
	execve(p_cmd, arg, s->envp);
}

void	execute_child(t_sequ *seq, t_cmd *cmd, t_cmd *first_cmd)
{
	char		*path_cmd;

	if (cmd->redirect && !__handle_redirect(cmd))
		__exit_child(seq, first_cmd, 0, 0);
	if (!cmd->arg || !cmd->arg[0])
	{
		cmd->msh->rv = 0;
		__exit_child(seq, first_cmd, 0, 0);
	}
	if (__is_builtin(cmd->arg))
	{
		__exec_builtin(cmd->arg, cmd->msh, cmd, 1);
		__exit_child(seq, first_cmd, cmd->msh->rv, 0);
	}
	path_cmd = __get_path(seq->path, cmd->arg[0]);
	if (!path_cmd)
		__check_access_and_exit(cmd->arg[0], seq, first_cmd);
	if (path_cmd)
		__check_access_exe(path_cmd, cmd->arg, seq, first_cmd);
	__exit_child(seq, first_cmd, errno, 0);
}
