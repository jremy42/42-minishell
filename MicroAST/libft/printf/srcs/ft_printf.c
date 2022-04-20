/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 08:55:20 by jremy             #+#    #+#             */
/*   Updated: 2021/12/10 11:28:11 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

t_flags	__init_flags(void)
{
	t_flags	flags;

	flags.type = '0';
	flags.minus = 0;
	flags.dot = 0;
	flags.ndot = 0;
	flags.blank = 0;
	flags.hash = 0;
	flags.zero = 0;
	flags.plus = 0;
	flags.widht = 0;
	flags.negatif = 0;
	flags.value = 0;
	flags.nullbit = 0;
	flags.content = NULL;
	return (flags);
}

int	__parsing(char *src, va_list args)
{
	t_flags	flags;
	int		i;
	int		prtc;

	i = 1;
	prtc = 0;
	flags = __init_flags();
	if (__is_flag(src[i]))
		i = __parsing_flags(src + i, &flags);
	if (__valide_type(src[i]))
		i = __parsing_type(src[i], &flags, args);
	if (!flags.content)
		return (0);
	if (i > 0)
		prtc = __print(&flags);
	if (flags.content)
		free(flags.content);
	if (prtc == -1)
		return (0);
	return (prtc);
}

int	__treat(char *src, va_list args)
{
	int	i;
	int	prtc;

	i = 0;
	prtc = 0;
	while (src[i])
	{
		if (src[i] == '%' && src[i + 1])
		{
			if (__valide_type(src[i + 1]) || __is_flag(src[i + 1]))
			{
				prtc += __parsing(src + i, args);
			}
			i += __next_i(src + i + 1);
		}
		else
		{
			__putchar(src[i]);
			i++;
			prtc++;
		}
	}
	return (prtc);
}

int	__printf(const char *str, ...)
{
	char	*memory;
	va_list	args;
	int		return_value;

	memory = __strdup(str);
	va_start(args, str);
	return_value = 0;
	return_value += __treat(memory, args);
	va_end(args);
	free(memory);
	return (return_value);
}
