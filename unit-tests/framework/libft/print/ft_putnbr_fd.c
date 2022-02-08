/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:47:47 by jremy             #+#    #+#             */
/*   Updated: 2022/01/10 15:04:51 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	__putnbr_recu_fd(unsigned int sn, int fd)
{
	char	c;

	if (sn < 10)
	{
		c = sn + '0';
		__putchar_fd(c, fd);
		return ;
	}
	__putnbr_recu_fd(sn / 10, fd);
	c = sn % 10 + '0';
	__putchar_fd(c, fd);
}

void	__putnbr_fd(int n, int fd)
{
	unsigned int	sn;

	if (n < 0)
	{
		__putchar_fd('-', fd);
		sn = n * -1;
	}
	else
		sn = n;
	__putnbr_recu_fd(sn, fd);
}
