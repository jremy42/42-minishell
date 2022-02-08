/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:30:17 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*__memmove(void *dst, const void *src, size_t size)
{
	unsigned char	*d;
	unsigned char	*s;

	if (!dst && !src)
		return (dst);
	if (dst < src)
	{
		d = (unsigned char *)dst;
		s = (unsigned char *)src;
		while (size--)
			*d++ = *s++;
	}
	else
	{
		d = (unsigned char *)dst + (size - 1);
		s = (unsigned char *)src + (size - 1);
		while (size--)
			*d-- = *s--;
	}
	return (dst);
}
