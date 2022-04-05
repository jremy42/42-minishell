/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:19:06 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 12:57:44 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

extern int	g_rv;

static int	__end_of_retrieve_hd(int fd, char *hd_content, t_lexing *lexing,
		int eof_type)
{
	if (close(fd) < 0)
		return (free(hd_content), 0);
	if (unlink(".hd.tmp") < 0)
		return (free(hd_content), 0);
	free(lexing->next->token);
	lexing->next->token = hd_content;
	lexing->next->hd_type = eof_type;
	return (1);
}

int	__retrieve_hd(t_lexing *lexing, int eof_type)
{
	char	*hd_content;
	int		fd;
	char	buf[32];
	int		read_status;

	hd_content = __strdup("");
	if (!hd_content)
		return (0);
	fd = open(".hd.tmp", O_RDONLY);
	if (fd < 0)
		return (free(hd_content), 0);
	read_status = 1;
	while (read_status)
	{
		read_status = read(fd, buf, 32);
		if (read_status < 0)
			return (free(hd_content), close (fd), unlink("hd.tmp"), 0);
		buf[read_status] = '\0';
		hd_content = __strjoin(hd_content, buf);
		if (!hd_content)
			return (close (fd), unlink("hd.tmp"), 0);
	}
	return (__end_of_retrieve_hd(fd, hd_content, lexing, eof_type));
}

static int	wait_here_doc(pid_t pid, t_msh *msh)
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
	if (ret == 130)
		msh->rv = 130;
	return (ret);
}

int	__handle_here_doc(t_lexing *lexing, t_lexing *end, t_msh *msh)
{
	pid_t		pid;
	char		*eof;
	t_lexing	*save;

	save = lexing;
	while (lexing != end && lexing && lexing->next != end)
	{
		if (lexing->type == HERE_DOC)
		{
			eof = lexing->next->token;
			pid = fork();
			if (pid == 0)
				__init_child_hd(eof, lexing, msh, save);
			else
			{
				if (wait_here_doc(pid, msh) == 130)
					return (130);
				if (!__retrieve_hd(lexing, __eof_type(eof)))
					return (0);
				lexing = lexing->next;
			}
		}
		lexing = lexing->next;
	}
	return (1);
}

int	here_doc_handler(t_user_input *ui, t_msh *msh)
{
	ui->ret_hd = __handle_here_doc(ui->lexing, ui->first_error, msh);
	if (ui->ret_hd == 0)
		return (__lex_fclear(&ui->lexing),
			__exit_error(msh, 3, "here_doc"));
	if (ui->ret_hd == 130)
		return (__lex_fclear(&ui->lexing), 0);
	return (1);
}
