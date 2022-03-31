/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 11:44:30 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 11:46:14 by jremy            ###   ########.fr       */
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

int	__sub_cdnte(char *env_key, char *candidate,
	char **expanded_token, char *env_value)
{
	free(env_key);
	free(candidate);
	*expanded_token = __strjoin(*expanded_token, env_value);
	if (!*expanded_token)
		return (0);
	return (1);
}

char	*__get_key_from_key_val(char *str)
{
	return (__substr(str, 0, __strchr(str, '=') - str));
}

int	__key_match_canditate(char *cndte, char *env_key, t_msh *msh, int j)
{
	if (!__strcmp(cndte, env_key) && msh->envp[j][1] &&
		__strchr(msh->envp[j][0], '='))
		return (1);
	return (0);
}
