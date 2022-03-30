/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher_forks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:23:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/30 12:43:18 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static int	__first_pipe(int pipe[2])
{
	close(pipe[0]);
	if (dup2(pipe[out], STDOUT_FILENO) < 0)
		return (0);
	close(pipe[out]);
	return (1);
}

static int	__middle_pipe(int pipe[2], int prev_pipe_out)
{
	if (dup2(pipe[out], STDOUT_FILENO) < 0)
		return (0);
	close(pipe[out]);
	close(pipe[in]);
	if (dup2(prev_pipe_out, 0) < 0)
		return (0);
	close(prev_pipe_out);
	return (1);
}

static int	__last_pipe(int pipe[2], int prev_pipe_out)
{
	DEBUG && fprintf(stderr, "last pipe\n");
	if (dup2(prev_pipe_out, 0) < 0)
		return (0);
	close(pipe[out]);
	close(prev_pipe_out);
	return (1);
}

static int	wait_ret(pid_t pid)
{
	int	status;
	int	ret;

	ret = 0;
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) > 0)
		ret = (WEXITSTATUS(status));
	if (WIFSIGNALED(status) > 0)
		ret = (WTERMSIG(status) + 128);
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (ret);
}

void	__init_child(t_sequ *seq, t_cmd *cmd, t_cmd *first_cmd, int pv_pipe_out)
{
	DEBUG && fprintf(stderr, "I m a fork\n");
	if (seq->index == 0 && seq->max_cmd != 1)
	{
		if (!__first_pipe(seq->pipe))
			__exit_child(seq, first_cmd, 126, errno);
	}
	if (seq->index == seq->max_cmd - 1 && seq->max_cmd != 1)
	{
		if (!__last_pipe(seq->pipe, pv_pipe_out))
			__exit_child(seq, first_cmd, 126, errno);
	}
	else if (seq->max_cmd != 1 && seq->index != 0)
	{
		if (!__middle_pipe(seq->pipe, pv_pipe_out))
			__exit_child(seq, first_cmd, 126, errno);
	}
	execute_child(seq, cmd, first_cmd);
}

int	__launcher_fork(t_sequ *seq, t_cmd *cmd, t_cmd *first_cmd)
{
	pid_t	pid;
	int		prev_pipe_out;

	prev_pipe_out = 0;
	while (seq->index < seq->max_cmd)
	{
		if (seq->index < seq->max_cmd - 1 && pipe(seq->pipe) < 0)
			return (__putstr_fd("Minishell: Error pipe\n", 2), 1);
		pid = fork();
		if (pid < 0)
			return (__putstr_fd("Minishell: Error fork\n", 2), 1);
		if (pid == 0)
			__init_child(seq, cmd, first_cmd, prev_pipe_out);
		else
		{
			close(seq->pipe[1]);
			if (seq->index > 0)
				close(prev_pipe_out);
			prev_pipe_out = seq->pipe[0];
			seq->index++;
			cmd = cmd->next;
		}
	}
	return (wait_ret(pid));
}
