/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __parsing_type_hex.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 10:54:51 by jremy             #+#    #+#             */
/*   Updated: 2021/12/03 19:03:07 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

void	str_to_upper(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		str[i] = __toupper(str[i]);
		i++;
	}
}

void	__return_hexa(unsigned int x, t_flags *flags, char type)
{
	char	*tmp;

	flags->type = type;
	if (x == 0 && flags->dot)
	{
		flags->content = __strdup("");
		return ;
	}
	tmp = __str_convert(x, "0123456789abcdef");
	if (x == 0)
		flags->hash = 0;
	if (type == 'X')
		str_to_upper(tmp);
	flags->content = tmp;
}
