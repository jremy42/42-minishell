/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 10:42:42 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	__isinset(unsigned char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*__strtrim(char const *s1, char const *set)
{
	char	*dst;
	int		start;
	int		size;

	if (!s1 || !set)
		return (NULL);
	size = (int)__strlen(s1);
	start = 0;
	while (s1[start] && __isinset(s1[start], set))
		start++;
	while ((size > start) && __isinset(s1[size - 1], set))
		size--;
	dst = __calloc(size - start + 1, sizeof(char));
	if (!dst)
		return (NULL);
	__memcpy(dst, &s1[start], size - start);
	return (dst);
}
