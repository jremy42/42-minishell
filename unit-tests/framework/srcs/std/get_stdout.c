/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_stdout.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 12:37:14 by jremy             #+#    #+#             */
/*   Updated: 2022/01/09 17:41:13 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

int	__init_get(t_pipe *std)
{
	std->stdout_cpy = dup(STDOUT_FILENO);
	std->stdin_cpy = dup(STDIN_FILENO);
	if (pipe(std->pipe_fds) == -1)
	{
		close(std->stdout_cpy);
		close(std->stdin_cpy);
		return (-1);
	}
	if (dup2(std->pipe_fds[0], STDIN_FILENO) == -1
		|| dup2(std->pipe_fds[1], STDOUT_FILENO) == -1)
	{
		__close_std(std);
		return (-1);
	}
	return (0);
}

void	__read_std(char *ret, t_pipe *std)
{
	char	buffer[BUFFER_SIZE];

	fflush(stdout);
	bzero((void *)buffer, BUFFER_SIZE);
	read(std->pipe_fds[0], buffer, BUFFER_SIZE);
	strcpy(ret, buffer);
}

void	__close_std(t_pipe *std)
{
	if (std->pipe_fds[0] != -1)
		close(std->pipe_fds[0]);
	if (std->pipe_fds[1] != -1)
		close(std->pipe_fds[1]);
	dup2(std->stdout_cpy, STDOUT_FILENO);
	dup2(std->stdin_cpy, STDIN_FILENO);
	close(std->stdout_cpy);
	close(std->stdin_cpy);
}
