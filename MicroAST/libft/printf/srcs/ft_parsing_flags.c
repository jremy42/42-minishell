/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __parsing_flags.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 09:25:00 by jremy             #+#    #+#             */
/*   Updated: 2021/12/09 08:55:56 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int	__treatdigit(char *str, int i, t_flags *flags)
{
	flags->widht = __atoi(str + i);
	while (__isdigit(str[i]))
		i++;
	return (i - 1);
}

int	__flag_dot(char *str, int i, t_flags *flags)
{
	i++;
	flags->dot = 1;
	if (!__isdigit(str[i]))
	{
		flags->ndot = 0;
		return (i);
	}
	else
	{
		flags->ndot = __atoi(str + i);
		while (__isdigit(str[i]) && str[i])
			i++;
		return (i);
	}
}

int	__flag_minus(char *str, int i, t_flags *flags)
{
	i++;
	flags->minus = 1;
	while (str[i] == '-')
		i++;
	if (!__isdigit(str[i]))
		return (i);
	else
	{
		flags->widht = __atoi(str + i);
		while (__isdigit(str[i]) && str[i])
			i++;
		return (i);
	}
}

int	__flag_zero(char *str, int i, t_flags *flags)
{
	i++;
	flags->zero = 1;
	while (str[i] == '0')
		i++;
	if (!__isdigit(str[i]))
	{
		flags->zero = 0;
		return (i);
	}
	else
	{
		flags->widht = __atoi(str + i);
		while (__isdigit(str[i]) && str[i])
			i++;
		return (i);
	}
}

int	__parsing_flags(char *str, t_flags *flags)
{
	int	i;

	i = 0;
	while (str[i] && __is_flag(str[i]))
	{	
		if (str[i] == ' ')
			flags->blank = 1;
		if (str[i] == '#')
			flags->hash = 1;
		if (str[i] == '+')
			flags->plus = 1;
		if (str[i] == '0')
			i = __flag_zero(str, i, flags);
		if (str[i] == '-')
			i = __flag_minus(str, i, flags);
		if (str[i] == '.')
			i = __flag_dot(str, i, flags);
		if ((__isdigit(str[i])) && flags->dot == 0)
			i = __treatdigit(str, i, flags);
		if (__valide_type(str[i]) || str[i] == '%')
			break ;
		else
			i++;
	}
	return (__count_flags(str));
}
