/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 15:34:31 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 16:23:40 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int	__valide_type(char type)
{
	if (type == 'c' || type == 's' || type == 'p' || type == 'd' || type == 'i'
		|| type == 'u' || type == 'x' || type == 'X' || type == '%')
		return (1);
	else
		return (0);
}

int	__is_flag(char flag)
{
	if (flag == '.' || flag == ' ' || flag == '#' || flag == '0'
		|| flag == '-' || flag == '+' || __isdigit(flag))
		return (1);
	else
		return (0);
}

void	__putchar(int c)
{
	write(1, &c, 1);
}

void	__return_percent(t_flags *flags)
{
	char	*ret;

	ret = __calloc(2, sizeof(char));
	if (!ret)
		return ;
	ret[0] = '%';
	ret[1] = '\0';
	flags->content = __strdup(ret);
	free(ret);
	flags->type = '%';
}

int	__next_i(char *src)
{
	int	i;

	i = 0;
	while (__is_flag(src[i]) && src[i])
		i++;
	if (__valide_type(src[i]) && src[i])
		i++;
	return (i + 1);
}
