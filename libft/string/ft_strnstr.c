/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   __strnstr.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: jremy <jremy@student.42.fr>+#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2021/11/23 17:48:19 by jremy #+##+# */
/*   Updated: 2022/01/10 15:04:51 by jremy###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

char	*__strnstr(const char *str, const char *find, size_t len)
{
	size_t		find_len;
	size_t		i;
	const char	*p;

	i = 0;
	p = str;
	if (*find == '\0')
		return ((char *)p);
	find_len = __strlen(find);
	if (*p == '\0' || len == 0 || __strlen(str) < find_len)
		return (NULL);
	if (*find == '\0')
		return ((char *)p);
	while (*p != '\0' && i <= (len - find_len))
	{
		if (*p == *find)
		{
			if (__strncmp((char *)p, (char *)find, find_len) == 0)
				return ((char *)p);
		}
		p++;
		i++;
	}
	return (NULL);
}

char	*__strstr(const char *str, const char *find)
{
	size_t		i;
	size_t		find_len;

	i = 0;
	if (*find == '\0')
		return ((char *)str);
	find_len = __strlen(find);
	if (*str == '\0')
		return (NULL);
	if (*find == '\0')
		return ((char *)str);
	while (*str != '\0' )
	{
		if (*str == *find)
		{
			if (__strncmp((char *)str, (char *)find, find_len) == 0)
				return ((char *)str);
		}
		str++;
		i++;
	}
	return (NULL);
}

char	*__strrstr(const char *str, const char *find)
{
	int		find_len;
	int		str_len;

	if (*find == '\0')
		return ((char *)str);
	find_len = (int)__strlen(find);
	if (*str == '\0')
		return (NULL);
	str_len = (int)__strlen(str);
	if (find_len > str_len)
		return (NULL);
	while (--str_len >= 0)
	{
		if (str[str_len] == find[0])
		{
			if (__strncmp((char *)str + str_len, (char *)find, find_len) == 0)
				return ((char *)str + str_len);
		}
	}
	return (NULL);
}
