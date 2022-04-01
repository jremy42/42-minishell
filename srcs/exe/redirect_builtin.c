/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 09:51:56 by jremy             #+#    #+#             */
/*   Updated: 2022/04/01 16:59:29 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

int	__test_access_write(t_cmd *cmd)
{
	if (access(cmd->redirect->file_name, F_OK) == 0
		&& access(cmd->redirect->file_name, W_OK) < 0)
	{
		cmd->msh->rv = 126;
		print_error(cmd->redirect->file_name, "Permission denied", NULL);
		return (0);
	}
	return (1);
}

int	__test_access_read(t_cmd *cmd)
{
	if (access(cmd->redirect->file_name, F_OK) < 0)
	{
		cmd->msh->rv = 127;
		print_error(cmd->redirect->file_name,
			"No such file or directory", NULL);
		return (0);
	}
	else if (access(cmd->redirect->file_name, R_OK) < 0)
	{
		cmd->msh->rv = 126;
		print_error(cmd->redirect->file_name, "Permission denied", NULL);
		return (0);
	}
	return (1);
}

static int	__less_redirection(t_cmd *cmd)
{
	if (cmd->redirection[0] != -1)
		close(cmd->redirection[0]);
	if (!__test_access_read(cmd))
		return (0);
	cmd->redirection[0] = open(cmd->redirect->file_name, O_RDONLY);
	if (cmd->redirection[0] < 0)
		return (print_error(cmd->redirect->file_name,
				"No such file or directory", NULL), 0);
	if (dup2(cmd->redirection[0], 0) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	if (close(cmd->redirection[0]) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	return (1);
}

static int	__great_redirection(t_cmd *cmd)
{
	if (cmd->redirection[1] != -1)
		close(cmd->redirection[1]);
	if (!__test_access_write(cmd))
		return (0);
	cmd->redirection[1] = open(cmd->redirect->file_name,
			O_CREAT | O_WRONLY | O_TRUNC, 00644);
	if (cmd->redirection[1] < 0)
		return (print_error(cmd->redirect->file_name,
				"No such file or directory", NULL), 0);
	if (dup2(cmd->redirection[1], 1) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	if (close(cmd->redirection[1]) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	return (1);
}

static int	__dgreat_redirection(t_cmd *cmd)
{
	if (cmd->redirection[1] != -1)
		close(cmd->redirection[1]);
	if (!__test_access_write(cmd))
		return (0);
	cmd->redirection[1] = open(cmd->redirect->file_name,
			O_CREAT | O_WRONLY | O_APPEND, 00644);
	if (cmd->redirection[1] < 0)
		return (print_error(cmd->redirect->file_name,
				"No such file or directory", NULL), 0);
	if (dup2(cmd->redirection[1], 1) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	if (close(cmd->redirection[1]) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	return (1);
}

static int	__here_doc_redirection(t_cmd *cmd)
{
	char	*tmp_file;
	int		fd;

	tmp_file = __get_name(cmd->index);
	if (!tmp_file)
		return (0);
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 00644);
	if (fd < 0)
		return (__putendl_fd(strerror(errno), 2), free(tmp_file), 0);
	__putstr_fd(cmd->redirect->file_name, fd);
	close(fd);
	cmd->redirection[0] = open(tmp_file, O_RDONLY);
	if (cmd->redirection[0] < 0)
		return (free(tmp_file), 0);
	free(tmp_file);
	if (cmd->redirection[0] < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	if (dup2(cmd->redirection[0], 0) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	if (close(cmd->redirection[0]) < 0)
		return (__putendl_fd(strerror(errno), 2), 0);
	return (1);
}

int	__reset_cmd_start(t_cmd *cmd, t_redirect *save)
{
	cmd->redirect = save;
	return (0);
}

int	__handle_redirect_builtin(t_cmd *cmd)
{
	t_redirect	*save;

	save = cmd->redirect;
	while (cmd->redirect)
	{
		if (cmd->redirect->type == LESS && !__less_redirection(cmd))
			return (__reset_cmd_start(cmd, save));
		if (cmd->redirect->type == GREAT && !__great_redirection(cmd))
			return (__reset_cmd_start(cmd, save));
		if (cmd->redirect->type == DGREAT && !__dgreat_redirection(cmd))
			return (__reset_cmd_start(cmd, save));
		if (cmd->redirect->type == H_D && ! __here_doc_redirection(cmd))
			return (__reset_cmd_start(cmd, save));
		cmd->redirect = cmd->redirect->next;
	}
	cmd->redirect = save;
	return (1);
}