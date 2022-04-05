/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 17:55:52 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/05 12:54:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__is_valid_name_char(char c)
{
	if (__isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*__get_key_from_key_val(char *str)
{
	return (__substr(str, 0, __strchr(str, '=') - str));
}

int	__split_expanded_token(t_lexing *lexing)
{
	char	**split_token;
	int		i;

	i = 0;
	split_token = __split(lexing->token, ' ');
	if (!split_token)
		return (0);
	while (split_token[i])
	{
		if (!__insert_token(lexing, split_token[i], 0, NULL))
			return (free_split(split_token), 0);
		i++;
	}
	free_split(split_token);
	__insert_token(NULL, NULL, 1, NULL);
	return (1);
}
