/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:54:48 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	__intsize(int n, int *sign)
{
	size_t			result;
	unsigned int	tmp;

	result = 0;
	if (n < 0)
	{
		tmp = n * -1;
		*sign = 1;
	}
	else
		tmp = n;
	while (tmp > 0)
	{
		tmp = tmp / 10;
		result++;
	}
	return (result + *sign);
}

static void	__itoa_recu(unsigned int sn, char *result, size_t start)
{
	if (sn < 10)
	{
		result[start] = sn + '0';
		return ;
	}
	__itoa_recu(sn / 10, result, start - 1);
	result[start] = sn % 10 + '0';
}

char	*__itoa(int n)
{
	char			*result;
	size_t			size;
	unsigned int	sn;
	int				sign;

	sign = 0;
	if (n == 0)
	{
		result = __calloc(2, sizeof(char));
		result[0] = '0';
		return (result);
	}
	size = __intsize(n, &sign);
	if (sign == 1)
		sn = n * -1;
	else
		sn = n;
	result = __calloc((size + 1), sizeof(char));
	if (!result)
		return (NULL);
	__itoa_recu(sn, result, size - 1);
	if (sign == 1)
		result[0] = '-';
	return (result);
}
