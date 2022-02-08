/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:41:48 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	__strnlen(char *dst, size_t n)
{
	size_t	i;

	i = 0;
	while (i <= n && dst[i])
		i++;
	return (i);
}

size_t	__strlcat(char *dst, const char *src, size_t n)
{
	size_t	len_dst;
	size_t	len_src;
	size_t	result;
	size_t	i;

	len_dst = __strnlen(dst, n);
	len_src = __strlen(src);
	i = 0;
	if (n == 0)
		return (len_src);
	if (n < len_dst)
		return (len_src + n);
	else
		result = len_src + len_dst;
	while ((len_dst + i < n - 1) && src[i])
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (result);
}
