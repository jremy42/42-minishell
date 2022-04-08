/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:53:22 by jremy             #+#    #+#             */
/*   Updated: 2022/04/08 12:24:35 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	__parameter_expand_dq(char *start_word, t_msh *msh,
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
				return (__sub_cdnte_dquotes(env_key, cndte, expanded_token,
						__strchr(msh->envp[j][0], '=') + 1));
			free(env_key);
		}
		j++;
	}
	return (free(cndte), 1);
}

int	__expand_word(char **token_word, t_msh *msh)
{
	char	*tmp;
	int		i;
	char	*expd_tken_wd;

	expd_tken_wd = __strdup("");
	if (!expd_tken_wd)
		return (0);
	tmp = *token_word;
	i = -1;
	while (tmp[++i])
	{
		if ((tmp[i] == '$' && !__get_dollar_status(tmp, &tmp[i]))
			&& (__is_valid_name_char(tmp[i + 1]) || __is_an_exception(tmp, i)))
		{
			if (__is_inside_quotes(tmp, &tmp[i])
				&& !__parameter_expand_dq(tmp + i + 1, msh, &expd_tken_wd, &i))
				return (free(expd_tken_wd), 0);
			else if (!__parameter_expand(tmp + i + 1, msh, &expd_tken_wd, &i))
				return (free(expd_tken_wd), 0);
		}
		else if (!__add_char_to_token(tmp[i], &expd_tken_wd))
			return (free(expd_tken_wd), 0);
	}
	__update_token_word(token_word, &expd_tken_wd);
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
