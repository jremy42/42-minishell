/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 10:04:44 by jremy             #+#    #+#             */
/*   Updated: 2022/04/04 19:01:12 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*__get_prompt(int rv)
{
	char	path[PATH_MAX];
	char	*prompt;

	__bzero(path, PATH_MAX);
	if (rv == 0)
		prompt = __strdup("➜  ");
	else
		prompt = __strdup("x  ");
	if (!prompt)
		return (NULL);
	if (getcwd(path, PATH_MAX))
	{
		prompt = __strjoin2(prompt,
				path + __get_pos_last_dir(path), "  ~  ");
		if (!prompt)
			return (NULL);
	}
	else
		prompt = __strjoin(prompt, RESET);
	return (prompt);
}

int	__prevent_rl_write_stdout(int *fd, int *save_stdout)
{
	*fd = open("/dev/null", O_WRONLY);
	if (*fd < 0)
		return (0);
	*save_stdout = dup(1);
	if (*save_stdout < 0)
		return (0);
	if (dup2(*fd, 1) < 0)
		return (close (*fd), 0);
	close(*fd);
	return (1);
}

int	__restore_rl_write_stdout(int save_stdout)
{
	if (dup2(save_stdout, 1) < 0)
		return (close (save_stdout), 0);
	close (save_stdout);
	return (1);
}

char	*__exe_readline(int rv)
{
	char	*arg;
	char	*prompt;
	int		fd;
	int		save_stdout;

	prompt = NULL;
	if (!isatty(0) && !__prevent_rl_write_stdout(&fd, &save_stdout))
		return (NULL);
	if (isatty(2))
	{
		prompt = __get_prompt(rv);
		if (!prompt)
			return (print_error("prompt", "Malloc error", NULL), NULL);
	}
	arg = readline(prompt);
	free(prompt);
	if (!isatty(0) && !__restore_rl_write_stdout(save_stdout))
		return (NULL);
	return (arg);
}
