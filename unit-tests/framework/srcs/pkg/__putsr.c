/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __putsr.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 19:28:06 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 15:56:30 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

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
