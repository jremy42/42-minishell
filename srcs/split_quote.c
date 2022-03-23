/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:10:32 by jremy             #+#    #+#             */
/*   Updated: 2022/03/23 18:35:50 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**__freetab(char**tab, int index)
{
	int	i;

	i = 0;
	if (tab)
	{
		while (i <= index)
		{
			free(tab[i]);
			tab[i] = NULL;
			i++;
		}
	}
	free(tab);
	return (NULL);
}

size_t __is_in_charset(char c, char *charset)
{
	int i;

	i = 0;

	while(charset[i])
	{
		if(charset[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static size_t	__count_word(char *s, char *charset)
{
	size_t	i;
	size_t	count_word;

	count_word = 0;
	i = 0;
	while (s[i])
	{
		if (!__is_in_charset(s[i], charset))
		{
			count_word++;
			while (s[i] && (!__is_in_charset(s[i], charset) || __get_char_quote_status(s, &s[i])))
				i++;
		}
		else
			i++;
	}
	return (count_word);
}

static char	*__getnextstr(char *s, char *charset, size_t *len_nextstr)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (!__is_in_charset(s[i], charset))
		{
			while (s[i] && (!__is_in_charset(s[i], charset) || __get_char_quote_status(s, &s[i])))		
			{
				i++;
				len++;
			}
			*len_nextstr = len;
			break ;
		}
		i++;
	}
	return ((char *)s + i - len);
}


char	**__split_unquoted_charset(char *s, char *charset)
{
	char	**result;
	size_t	count_word;
	size_t	i;
	size_t	len_nextstr;

	i = 0;
	len_nextstr = 0;
	if (!s)
		return (NULL);
	count_word = __count_word(s, charset);
	result = malloc(sizeof(char *) * (count_word + 1));
	if (!result)
		return (NULL);
	while (i < count_word)
	{
		s = __getnextstr(s + len_nextstr, charset, &len_nextstr);
		result[i] = __calloc(len_nextstr + 1, sizeof(char));
		if (! result[i])
			return (__freetab(result, i));
		result[i] = __strncpy(result[i], s, len_nextstr);
		result[i][len_nextstr] = '\0';
		i++;
	}
	result[count_word] = NULL;
	return (result);
}
