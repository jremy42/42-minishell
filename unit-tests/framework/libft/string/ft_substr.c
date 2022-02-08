/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 18:02:06 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*__substr(char const *s, unsigned int start, size_t len)
{
	char	*result;
	size_t	lenstr;
	size_t	n;

	if (!s)
		return (NULL);
	n = 0;
	if (start >= __strlen(s))
		lenstr = 0;
	else if (__strlen(s) - start < len)
		lenstr = __strlen(s) - start;
	else
		lenstr = len;
	result = __calloc(lenstr + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (s[start] && n < lenstr)
	{
		result[n] = s[start];
		n++;
		start++;
	}
	result[n] = '\0';
	return (result);
}
