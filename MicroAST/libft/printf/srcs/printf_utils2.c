/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 15:34:31 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 16:23:40 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int	__count_flags(char *str)
{
	int	i;

	i = 0;
	while (__is_flag(str[i]))
		i++;
	return (i + 1);
}

int	__mfw_size(t_flags *flags)
{
	int	mfw;

	mfw = 0;
	mfw = flags->widht - flags->negatif - flags->plus - flags->blank;
	mfw -= __strlen(flags->content);
	return (mfw);
}

void	__putstr(char const *s)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		__putchar(s[i]);
		i++;
	}
}
