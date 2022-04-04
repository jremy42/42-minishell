/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 10:15:21 by jremy             #+#    #+#             */
/*   Updated: 2022/04/04 10:16:16 by jremy            ###   ########.fr       */
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

int	__reset_cmd_start(t_cmd *cmd, t_redirect *save)
{
	cmd->redirect = save;
	return (0);
}
