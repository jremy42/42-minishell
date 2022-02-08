/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:57:30 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*__strchr(const char *src, int find)
{
	size_t			i;
	unsigned char	*s1;
	size_t			size;

	i = 0;
	size = __strlen(src);
	if (!src)
		return (NULL);
	s1 = (unsigned char *)src;
	while (i <= size)
	{
		if (s1[i] == (unsigned char)find)
			return ((char *)(s1 + i));
		i++;
	}
	return (NULL);
}
