/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:48:19 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*__strnstr(const char *str, const char *find, size_t len)
{
	size_t		find_len;
	size_t		i;
	const char	*p;

	i = 0;
	p = str;
	find_len = __strlen(find);
	if (*find == '\0')
		return ((char *)p);
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
