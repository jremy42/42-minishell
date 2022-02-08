/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __putsr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 19:28:06 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 17:51:16 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

static int	__strlen(char *str)
{
	char	*ptr;

	ptr = str;
	while (*++str)
		;
	return (str - ptr);
}

void	__putstr(char *str)
{
	write(1, str, __strlen(str));
}

void	__print_global_summary(int ok, int total)
{
	__putstr("Global summary = ");
	__putnbr(ok);
	__putstr("/");
	__putnbr(total);
	__putstr("\n");
}
