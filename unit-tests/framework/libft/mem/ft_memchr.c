/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:26:26 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*__memchr(const void *buffer, int c, size_t count)
{
	size_t			i;
	unsigned char	*src;

	src = (unsigned char *)buffer;
	i = 0;
	while (i < count)
	{
		if (*src == (unsigned char)c)
			return ((void *)src);
		src++;
		i++;
	}
	return (NULL);
}
