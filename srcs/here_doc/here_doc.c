/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 15:19:06 by jremy             #+#    #+#             */
/*   Updated: 2022/03/25 13:07:22 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static char	*__get_stdin(char *eof)
{
	char	*line;
	char	*ret;
	int		i;

	i = 0;
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
			if (!line)
				DEBUG && printf("warning: here-document at line %d\
				delimited by end-of-file (wanted `%s')\n",i, eof);
			get_next_line(-1);
			free(line);
			break ;
		}
		ret = __strjoin(ret, line);
		free (line);
		if (!ret)
			return (get_next_line(-1), NULL);
		i++;
	}
	return (ret);
}

static int	__get_user_input(char **eof)
{
	char	*stdin;
	int		file;
	int		quote;
	char	*tmp;
	
	quote = 0;
	if(((*eof)[0] == '"' && (*eof)[__strlen(*eof) - 1] == '"'))
	{
		tmp = __strtrim(*eof,"\"");
		if (!tmp)
			return (0);
		free(*eof);
		*eof = tmp;
		quote = 1;
	}
	else if(((*eof)[0] == '\'' && (*eof)[__strlen(*eof) - 1] == '\''))
	{
		tmp = __strtrim(*eof,"\'");
		if (!tmp)
			return (0);
		free(*eof);
		*eof = tmp;
		quote = 1;
	}
	file = open(".hd.tmp", O_CREAT | O_WRONLY | O_TRUNC, 00644);
	if (file < 0)
		return (-1);
	stdin = __get_stdin(*eof);
	if (!stdin)
		return (close(file), -1);
	if(quote)
		__putstr_fd("\'",file);
	__putstr_fd(stdin, file);
	if(quote)
		__putstr_fd("\'",file);
	free(stdin);
	close(file);
	return (1);
}

int		__retrieve_hd(t_lexing *lexing)
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
		buf[read_status]='\0';
		hd_content = __strjoin(hd_content, buf);
		if (!hd_content)
			return (close (fd), unlink("hd.tmp"), 0);
	}
	if (close(fd) < 0)
		return (free(hd_content), 0);
	if (unlink(".hd.tmp") < 0)
		return (free(hd_content), 0);
	free(lexing->next->token);
	lexing->next->token = hd_content;
	DEBUG && fprintf(stderr, "This is the heredoc : [%s]\n", hd_content);
	return (1);
}

int __handle_here_doc(t_lexing *lexing, t_lexing *end, t_msh *msh)
{
	pid_t       pid;
	char        *eof;
	t_lexing    *save;
	
	save = lexing;
	while (lexing != end)
	{
		if (lexing->type == HERE_DOC)
		{
			eof = lexing->next->token;
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				if (!__get_user_input(&eof))
					__putendl_fd("malloc error", 2);
				lexing->next->token = eof;
				msh->rv = errno;
				__lexing_full_list_clear(&save);
				__exit(msh);
			}
			else
			{
				waitpid(pid, NULL, 0);
				if(!__retrieve_hd(lexing))
					return (0);
				lexing = lexing->next;
			}
		}
		lexing = lexing->next;
	}
	return (1);
}
