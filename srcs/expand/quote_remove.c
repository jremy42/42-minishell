/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_remove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:10:26 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 12:15:37 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__quote_remove(char **token_word)
{
	char	*tmp;
	int		i;
	char	*quote_removed_token_word;

	quote_removed_token_word = __strdup("");
	if (!quote_removed_token_word)
		return (0);
	tmp = *token_word;
	i = 0;
	while (tmp[i])
	{
		if (((tmp[i] == '\\' && tmp[i + 1]) || tmp[i] == '\''
				|| tmp[i] == '"' ) && !__get_char_quote_status(tmp, &tmp[i]))
		{
			i++;
			continue ;
		}
		else
			if (!__add_char_to_token(tmp[i], &quote_removed_token_word))
				return (free(quote_removed_token_word), 0);
		i++;
	}
	free(*token_word);
	*token_word = quote_removed_token_word;
	return (1);
}

int	__quote_removal_token(t_lexing *lexing)
{
	while (lexing)
	{
		if (lexing->type == WORD)
		{
			if (!__quote_remove(&lexing->token))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		lexing = lexing->next;
	}
	return (1);
}

int	__quote_removal_glob(t_glob *glob)
{
	while (glob)
	{
		if (glob->to_find)
		{
			if (!__quote_remove(&glob->to_find))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		glob = glob->next;
	}
	return (1);
}

/*
int __quote_remove(char **token_word, t_msh  *msh)
{
	t_state slash_status;
	t_state quote_status;
	char *tmp;
	int     i;
	char    *quote_removed_token_word;
	(void)msh;
	
	quote_removed_token_word = __strdup("");
	if (!quote_removed_token_word)
		return (0);
	tmp = *token_word;
	quote_status = UNQUOTE;
	slash_status = 0;
	i = 0;
	while(tmp[i])
	{
		if (tmp[i] == '\\' && tmp[i + 1] && __need_to_escape(i, quote_status, tmp)
			&& !slash_status)
		{
			slash_status = BACKSLASH;
			i++;
			continue ;
		}
		if (quote_status != __return_state(tmp[i], quote_status, slash_status))
		{
			quote_status = __return_state(tmp[i], quote_status, slash_status);
			i++;
			continue ;
		}
		else
			if(!__add_char_to_token(tmp[i], &quote_removed_token_word))
				return(free(quote_removed_token_word), 0);
		i++;
		slash_status = 0;
	}
	free(*token_word);
	*token_word = quote_removed_token_word;
	return (1);
}
*/