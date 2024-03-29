/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher_forks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:23:45 by jremy             #+#    #+#             */
/*   Updated: 2022/04/14 11:02:03 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_rv;

static int	wait_ret(pid_t pid)
{
	int	status;
	int	ret;

	ret = 0;
	status = 0;
	g_rv = 0;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, __signal_waitpid);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) > 0)
		ret = (WEXITSTATUS(status));
	if (WIFSIGNALED(status) > 0)
		ret = (WTERMSIG(status) + 128);
	while (waitpid(-1, NULL, 0) > 0)
		;
	if (ret == 131 && WIFSIGNALED(status) > 0)
		__putendl_fd("Quit (core dumped)", 2);
	if (((ret == 130 && WIFSIGNALED(status) > 0) || g_rv == 130) && isatty(0))
		__putendl_fd("", 2);
	if (ret == 139 && WIFSIGNALED(status) > 0)
	{
		__putnbr_fd((int)pid, 2);
		__putendl_fd(" segmentation fault (core dumped)", 2);
	}
	return (signal(SIGINT, __signal), signal(SIGQUIT, __signal), ret);
}

int	__launcher_fork(t_sequ *seq, t_cmd *cmd, t_cmd *first_cmd)
{
	pid_t	pid;
	int		prev_pipe_out;

	prev_pipe_out = 0;
	while (seq->index < seq->max_cmd)
	{
		if (seq->index < seq->max_cmd - 1 && pipe(seq->pipe) < 0)
			return (__putstr_fd("minishell: Error pipe\n", 2), 129);
		pid = fork();
		if (pid < 0)
			return (__putstr_fd("minishell: Error fork\n", 2), 1);
		if (pid == 0)
			__init_child(seq, cmd, first_cmd, prev_pipe_out);
		else
		{
			if (seq->pipe[1] >= 0)
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
