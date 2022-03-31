/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:46:33 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 11:53:03 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_rv;

void	__treat_eof(char *line, char *eof, t_msh *msh)
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

char	*__get_stdin(char *eof, t_msh *msh)
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

int	__trim_quote(char **eof, int *quote)
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

int	__get_user_input(char **eof, t_msh *msh)
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

void	__init_child_hd(char *eof, t_lexing *lex, t_msh *msh, t_lexing *sv)
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
