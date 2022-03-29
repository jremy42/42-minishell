/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 14:53:33 by jremy             #+#    #+#             */
/*   Updated: 2022/03/29 17:27:25 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	__check_overflow(long long result)
{
	if (result < INT_MIN || result > INT_MAX)
		return (-1);
	return (0);
}

static int	__check_digit(const char *nb)
{
	int		i;
	int		check;

	i = 0;
	check = 0;
	while (nb[i])
	{
		if (__isdigit(nb[i]) != 1)
			return (-1);
		i++;
	}
	if (i == check)
		return (-1);
	return (0);
}

int	__atol(const char *nb, int *res)
{
	long long	result;
	int			sign;
	int			i;

	i = 0;
	sign = 1;
	result = 0;
	if (__check_digit(nb) == -1)
		return (0);
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
	*res = (int)result;
	return (1);
}
