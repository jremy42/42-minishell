/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __treat_zero.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 15:59:13 by jremy             #+#    #+#             */
/*   Updated: 2021/12/06 09:44:46 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "printf.h"

int	__addzero(char *src, int value, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		src[i] = value;
		i++;
	}
	src[i] = '\0';
	return (i);
}

int	__treat_zero(int mfw, t_flags *flags)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	tmp2 = NULL;
	tmp = malloc((mfw + 1) * sizeof(char));
	tmp[mfw] = '\0';
	if (!tmp)
		return (-1);
	__addzero(tmp, '0', mfw);
	tmp2 = flags->content;
	flags->content = __strjoin(tmp, tmp2);
	free(tmp2);
	free(tmp);
	return (1);
}
