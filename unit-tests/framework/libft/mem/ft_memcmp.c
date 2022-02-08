/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:44:32 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	__memcmp(const void *buffer1, const void *buffer2, size_t n)
{
	unsigned char	*b1;
	unsigned char	*b2;

	b1 = (unsigned char *)buffer1;
	b2 = (unsigned char *)buffer2;
	while (n--)
	{
		if (*b1 != *b2)
			return ((int)(*b1 - *b2));
		b1++;
		b2++;
	}
	return (0);
}
