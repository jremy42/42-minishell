/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher_forks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:23:45 by jremy             #+#    #+#             */
/*   Updated: 2022/03/11 12:18:49 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static int __first_pipe(int pipe[2], t_cmd *cmd)
{
    (void)cmd;	
    close(pipe[0]);
    dup2(pipe[out], STDOUT_FILENO);
	close(pipe[out]);
    return (1);
}

static int __middle_pipe(int pipe[2], int prev_pipe_out, t_cmd *cmd)
{
    (void)cmd;
    dup2(pipe[out], STDOUT_FILENO);
	close(pipe[out]);
    close(pipe[in]);
	dup2(prev_pipe_out, 0);
    close(prev_pipe_out);
    return (1);
}

static int __last_pipe(int pipe[2], int prev_pipe_out, t_cmd *cmd)
{
	(void)cmd;
    (void)pipe;
	 fprintf(stderr, "last pipe\n");
    dup2(prev_pipe_out, 0);
    close(pipe[out]);
	close(prev_pipe_out);
    return (1);
}

static int wait_ret(pid_t pid)
{
    int status;
    int ret;

    ret = 0;
    status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) > 0)
		ret = (WEXITSTATUS(status));
	if (WIFSIGNALED(status) > 0)
		ret = (WTERMSIG(status));
	while (waitpid(-1, NULL, 0) > 0)
		;
    return(ret);
}

int __launcher_fork(t_sequ *seq, t_cmd *cmd)
{
    pid_t pid;
   
    int prev_pipe_out;

    prev_pipe_out = 0;
    while (seq->index < seq->max_cmd)
    {
        if (seq->index < seq->max_cmd - 1 && pipe(seq->pipe) < 0)
            return (__putstr_fd("Error pipe\n", 2), 0);
        pid = fork();
        if (pid < 0)
            return (__putstr_fd("Error fork\n", 2), 0);
        if (pid == 0)
        {
            fprintf(stderr, "I m a fork\n");
			if (seq->index == 0 && seq->max_cmd != 1)
				__first_pipe(seq->pipe, cmd);
			if (seq->index == seq->max_cmd - 1 && seq->max_cmd != 1)
				__last_pipe(seq->pipe,prev_pipe_out, cmd);
            else if (seq->max_cmd != 1 && seq->index != 0)
                __middle_pipe(seq->pipe,prev_pipe_out, cmd);
            execute_child(seq, cmd);
        }
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
