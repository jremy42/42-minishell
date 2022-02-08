/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:57:59 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*__strrchr(const char *src, int find)
{
	unsigned char	*s1;
	int				i;

	i = (int)__strlen(src);
	s1 = (unsigned char *)src;
	while (i >= 0)
	{
		if (s1[i] == (unsigned char)find)
			return ((char *)(s1 + i));
		i--;
	}
	return (NULL);
}
