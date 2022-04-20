/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __parsing_type_ui.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 10:44:24 by jremy             #+#    #+#             */
/*   Updated: 2021/12/03 19:02:37 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

size_t	__u_intsize(unsigned int n)
{
	size_t		result;

	result = 0;
	while (n > 0)
	{
		n = n / 10;
		result++;
	}
	return (result);
}

void	__u_itoa_recu(unsigned int sn, char *result, size_t start)
{
	if (sn < 10)
	{
		result[start] = sn + '0';
		return ;
	}
	__u_itoa_recu(sn / 10, result, start - 1);
	result[start] = sn % 10 + '0';
}

char	*__u_itoa(unsigned int n)
{
	char		*result;
	size_t		size;

	if (n == 0)
	{
		result = __calloc(2, sizeof(char));
		result[0] = '0';
		return (result);
	}
	size = __u_intsize(n);
	result = __calloc(size + 1, sizeof(char));
	if (!result)
		return (NULL);
	__u_itoa_recu(n, result, size - 1);
	return (result);
}

void	__return_u_int(unsigned int d, t_flags *flags)
{
	if (d == 0 && flags->dot)
	{
		flags->content = __strdup("");
		return ;
	}
	flags->content = __u_itoa(d);
	flags->type = 'u';
}
