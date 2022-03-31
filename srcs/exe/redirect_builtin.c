/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 09:51:56 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 14:48:58 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static int	__less_redirection(t_cmd *cmd)
{
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

int	__handle_redirect_builtin(t_cmd *cmd)
{
	t_redirect	*save;

	save = cmd->redirect;
	DEBUG && fprintf(stderr, "Handle redirect\n");
	while (cmd->redirect)
	{
		if (cmd->redirect->type == LESS)
			if (!__less_redirection(cmd))
				return (0);
		if (cmd->redirect->type == GREAT)
			if (!__great_redirection(cmd))
				return (0);
		if (cmd->redirect->type == DGREAT)
			if (!__dgreat_redirection(cmd))
				return (0);
		if (cmd->redirect->type == H_D)
			if (!__here_doc_redirection(cmd))
				return (0);
		cmd->redirect = cmd->redirect->next;
	}
	cmd->redirect = save;
	return (1);
}
