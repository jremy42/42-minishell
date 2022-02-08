/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __putnbr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 10:24:09 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 15:05:55 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

static void	__putnbr_recu(unsigned int sn)
{
	char	c;

	if (sn < 10)
	{
		c = sn + '0';
		write (1, &c, 1);
		return ;
	}
	__putnbr_recu(sn / 10);
	c = sn % 10 + '0';
	write (1, &c, 1);
}

void	__putnbr(int n)
{
	unsigned int	sn;

	if (n < 0)
	{
		write (1, "-", 1);
		sn = n * -1;
	}
	else
		sn = n;
	__putnbr_recu(sn);
}
