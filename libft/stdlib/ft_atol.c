/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:53:33 by jremy             #+#    #+#             */
/*   Updated: 2022/03/29 17:50:46 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	__check_overflow(long long result)
{
	if (result < LLONG_MIN || result > LLONG_MAX)
		return (-1);
	return (0);
}

static int	__isisspace(char c)
{
	if ((c > 0 && c < 18) || (c == 32))
		return (1);
	return (0);
}

int	__atol(const char *nb, int *res)
{
	unsigned long long	result;
	int			sign;
	int			i;

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
		if (__check_overflow(result * sign) == -1)
			return (0);
		i++;
	}
	*res = (int)result * sign;
	return (1);
}
