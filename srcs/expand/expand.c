/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:53:22 by jremy             #+#    #+#             */
/*   Updated: 2022/04/06 11:00:44by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

int	__parameter_expand(char *start_word, t_msh *msh,
	char **expanded_token, int *i)
{
	char	*cndte;
	char	*env_key;
	int		j;

	if (start_word[0] == '?')
		return (__treat_last_rv(expanded_token, i, msh));
	cndte = __get_candidate(start_word, i);
	if (!cndte)
		return (0);
	j = 0;
	while (msh->envp[j])
	{
		if (msh->envp[j][1][0] == '1')
		{
			env_key = __get_key_from_key_val(msh->envp[j][0]);
			if (!env_key)
				return (free(cndte), 0);
			if (__key_match_canditate(cndte, env_key, msh, j))
				return (__sub_cdnte(env_key, cndte, expanded_token,
						__strchr(msh->envp[j][0], '=') + 1));
			free(env_key);
		}
		j++;
	}
	return (free(cndte), 1);
}

int	__get_dollar_status(char *str, char *to_find)
{
	t_state	slash_status;
	t_state	quote_status;
	int		i;

	quote_status = UNQUOTE;
	slash_status = 0;
	i = 0;
	while (str[i] && &str[i] != to_find)
	{
		if (str[i] == '\\'
			&& str[i + 1] && __need_to_escape(i, quote_status, str)
			&& !slash_status)
		{
			slash_status = BACKSLASH;
			i++;
			continue ;
		}
		quote_status = __return_state(str[i], quote_status, slash_status);
		i++;
		slash_status = 0;
	}
	if (!slash_status && (quote_status == UNQUOTE || quote_status == D_QUOTE))
		return (0);
	return (1);
}

int	__is_an_exception(char *tmp, int i)
{
	if (tmp[i + 1] && __strchr("?'", tmp[i + 1]))
		return (fprintf(stderr, "[1]\n"), 1);
	if (tmp[i + 1] && tmp[i + 1] == '"')
	{
		if (__get_char_quote_status(&tmp[i], tmp) && tmp[i + 1] == '"')
			return (fprintf(stderr, "[3]\n"), 0);
		else
			return (fprintf(stderr, "[4]\n"), 1);
	}
	return (fprintf(stderr, "[5]\n"), 0);
}

int	__expand_word(char **token_word, t_msh *msh)
{
	char	*tmp;
	int		i;
	char	*expanded_token_wd;

	expanded_token_wd = __strdup("");
	if (!expanded_token_wd)
		return (0);
	tmp = *token_word;
	i = -1;
	while (tmp[++i])
	{
		if ((tmp[i] == '$' && !__get_dollar_status(tmp, &tmp[i])) && (__is_valid_name_char(tmp[i + 1]) || __is_an_exception(tmp, i) ) )
		{
			if (!__parameter_expand(tmp + i + 1, msh, &expanded_token_wd, &i))
				return (free(expanded_token_wd), 0);
		}
		else if (!__add_char_to_token(tmp[i], &expanded_token_wd))
			return (free(expanded_token_wd), 0);
	}
	free(*token_word);
	*token_word = expanded_token_wd;
	return (1);
}

int	__expand_word_hd(char **token_word, t_msh *msh)
{
	char	*tmp;
	int		i;
	char	*expanded_token_wd;

	expanded_token_wd = __strdup("");
	if (!expanded_token_wd)
		return (0);
	tmp = *token_word;
	i = -1;
	while (tmp[++i])
	{
		if (tmp[i] == '$'
			&& (__is_valid_name_char(tmp[i + 1]) || tmp[i + 1] == '?'))
		{
			if (!__parameter_expand(tmp + i + 1, msh, &expanded_token_wd, &i))
				return (free(expanded_token_wd), 0);
		}
		else if (!__add_char_to_token(tmp[i], &expanded_token_wd))
			return (free(expanded_token_wd), 0);
	}
	free(*token_word);
	*token_word = expanded_token_wd;
	return (1);
}

int	__parameter_expand_token(t_lexing *lexing, t_msh *msh)
{
	while (lexing)
	{
		if (lexing->type == HERE_DOC)
		{	
			if (lexing->next->hd_type == 1
				&& !__expand_word_hd(&lexing->next->token, msh))
				return (__putendl_fd("Malloc error", 2), 0);
			lexing = lexing->next->next;
			continue ;
		}
		if (lexing->type == WORD)
		{
			if (!__expand_word(&lexing->token, msh))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		lexing = lexing->next;
	}
	return (1);
}
