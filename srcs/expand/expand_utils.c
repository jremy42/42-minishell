/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:44:30 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 12:52:42 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__treat_last_rv(char **expanded_token, int *i, t_msh *msh)
{
	char	*candidate;

	candidate = __itoa(msh->rv);
	if (!candidate)
		return (0);
	*i += 1;
	*expanded_token = __strjoin(*expanded_token, candidate);
	if (!*expanded_token)
		return (free(candidate), 0);
	return (free(candidate), 1);
}

char	*__get_candidate(char *start_word, int *i)
{
	char	*candidate;
	int		j;

	j = 0;
	while (__isalnum(start_word[j]) || start_word[j] == '_')
		j++;
	candidate = __substr(start_word, 0, j);
	if (!candidate)
		return (NULL);
	*i += j;
	return (candidate);
}

char	*__handle_quote_in_expand(char *env_value)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = __strdup("");
	if (!tmp)
		return (NULL);
	while (env_value[i])
	{
		if (env_value[i] == '\'' || env_value[i] == '"')
		{
			if (!__add_char_to_token('\\', &tmp))
				return (free(tmp), NULL);
		}
		if (!__add_char_to_token(env_value[i], &tmp))
			return (free(tmp), NULL);
		i++;
	}
	return (tmp);
}

int	__sub_cdnte(char *env_key, char *candidate,
	char **expanded_token, char *env_value)
{
	char	*tmp;

	free(env_key);
	free(candidate);
	tmp = __handle_quote_in_expand(env_value);
	if (!tmp)
		return (0);
	*expanded_token = __strjoin(*expanded_token, tmp);
	free(tmp);
	if (!*expanded_token)
		return (0);
	return (1);
}

int	__key_match_canditate(char *cndte, char *env_key, t_msh *msh, int j)
{
	if (!__strcmp(cndte, env_key) && msh->envp[j][1] &&
		__strchr(msh->envp[j][0], '='))
		return (1);
	return (0);
}
