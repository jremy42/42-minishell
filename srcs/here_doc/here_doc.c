/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:19:06 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 10:35:11 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

extern int	g_rv;

int	here_doc_handler(t_user_input *ui, t_msh *msh)
{
	ui->ret_hd = __handle_here_doc(ui->lexing, ui->first_error, msh);
	if (ui->ret_hd == 0)
		return (__lexing_full_list_clear(&ui->lexing),
			__exit_error(msh, 3, "here_doc"));
	if (ui->ret_hd == 130)
		return (__lexing_full_list_clear(&ui->lexing), 0);
	return (1);
}

static void	__treat_eof(char *line, char *eof, t_msh *msh)
{
	if (!line && g_rv)
	{
		get_next_line(-1);
		free(line);
		msh->rv = 130;
		return ;
	}
	if (!line)
	{
		__putstr_fd("warning: here-doc delimited by end-of-file (wanted`",
			2);
		__putstr_fd(eof, 2);
		__putstr_fd("')\n", 2);
	}
	get_next_line(-1);
	free(line);
}

static char	*__get_stdin(char *eof, t_msh *msh)
{
	char	*line;
	char	*ret;

	line = NULL;
	ret = __strdup("");
	if (!ret)
		return (NULL);
	while (1)
	{
		__putstr_fd("heredoc> ", 2);
		line = get_next_line(0);
		if (!line || (__strncmp(line, eof, __strlen(eof)) == 0
				&& __strlen(line) == __strlen(eof) + 1))
		{
			__treat_eof(line, eof, msh);
			break ;
		}
		ret = __strjoin(ret, line);
		free (line);
		if (!ret)
			return (get_next_line(-1), NULL);
	}
	return (ret);
}

static int	__trim_quote(char **eof, int *quote)
{
	char	*tmp;

	tmp = NULL;
	if (((*eof)[0] == '"' && (*eof)[__strlen(*eof) - 1] == '"'))
	{
		tmp = __strtrim(*eof, "\"");
		if (!tmp)
			return (0);
		free(*eof);
		*eof = tmp;
		*quote = 1;
	}
	else if (((*eof)[0] == '\'' && (*eof)[__strlen(*eof) - 1] == '\''))
	{
		tmp = __strtrim(*eof, "\'");
		if (!tmp)
			return (0);
		free(*eof);
		*eof = tmp;
		*quote = 1;
	}
	return (1);
}

static int	__get_user_input(char **eof, t_msh *msh)
{
	char	*stdin;
	int		file;
	int		quote;

	quote = 0;
	if (!__trim_quote(eof, &quote))
		return (0);
	file = open(".hd.tmp", O_CREAT | O_WRONLY | O_TRUNC, 00644);
	if (file < 0)
		return (-1);
	stdin = __get_stdin(*eof, msh);
	if (!stdin)
		return (close(file), -1);
	if (quote)
		__putstr_fd("\'", file);
	__putstr_fd(stdin, file);
	if (quote)
		__putstr_fd("\'", file);
	free(stdin);
	close(file);
	return (1);
}

static int	__end_of_retrieve_hd(int fd, char *hd_content, t_lexing *lexing)
{
	if (close(fd) < 0)
		return (free(hd_content), 0);
	if (unlink(".hd.tmp") < 0)
		return (free(hd_content), 0);
	free(lexing->next->token);
	lexing->next->token = hd_content;
	return (1);
}

int	__retrieve_hd(t_lexing *lexing)
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
	return (__end_of_retrieve_hd(fd, hd_content, lexing));
}

static void	__init_child_hd(char *eof, t_lexing *lex, t_msh *msh, t_lexing *sv)
{
	g_rv = 0;
	msh->rv = 0;
	signal(SIGINT, __signal_hd);
	signal(SIGQUIT, SIG_IGN);
	if (!__get_user_input(&eof, msh))
		__putendl_fd("minishell: malloc error in here doc", 2);
	lex->next->token = eof;
	__lexing_full_list_clear(&sv);
	__exit(msh);
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
	while (lexing != end)
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
				if (!__retrieve_hd(lexing))
					return (0);
				lexing = lexing->next;
			}
		}
		lexing = lexing->next;
	}
	return (1);
}
