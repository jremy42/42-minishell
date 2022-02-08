/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 17:49:42 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	__isisspace(char c)
{
	if ((c > 0 && c < 18) || (c == 32))
		return (1);
	return (0);
}

int	__atoi(const char *nb)
{
	size_t	result;
	size_t	sign;
	size_t	i;

	i = 0;
	sign = 1;
	result = 0;
	while (__isisspace(nb[i]))
		i++;
	if (nb[i] == '-' || nb[i] == '+')
	{
		if (nb[i] == '-')
			sign = -1;
		i++;
	}
	while (__isdigit(nb[i]) && nb[i])
	{
		result = result * 10 + (nb[i] - '0');
		i++;
	}
	return ((int)(result * sign));
}
