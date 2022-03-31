/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher_forks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:23:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 14:43:17 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

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
	if (ret == 131)
		__putendl_fd("Quit (core dumped)", 2);
	return (ret);
}

int	__launcher_fork(t_sequ *seq, t_cmd *cmd, t_cmd *first_cmd)
{
	pid_t	pid;
	int		prev_pipe_out;

	prev_pipe_out = 0;
	while (seq->index < seq->max_cmd)
	{
		if (seq->index < seq->max_cmd - 1 && pipe(seq->pipe) < 0)
			return (__putstr_fd("Minishell: Error pipe\n", 2), 129);
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
