/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 14:29:44 by jremy             #+#    #+#             */
/*   Updated: 2021/11/26 12:14:40 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

int	ft_size_line(char *str)
{
	int	i;

	i = 0;
	while ((str[i] != '\n') && (str[i] != '\0'))
		i++;
	if (str[i] == '\n')
		i++;
	return (i);
}

char	*ft_get_line(char *str)
{
	int		i;
	int		size;
	char	*result;

	if (!str)
		return (NULL);
	i = 0;
	size = ft_size_line(str);
	result = malloc(sizeof(char) * size + 1);
	if (!result)
		return (NULL);
	while ((str[i] != '\n' ) && str[i])
	{
		result[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		result[i] = '\n';
		result[i + 1] = '\0';
	}
	else
		result[i] = '\0';
	return (result);
}

char	*ft_strdup(char *src)
{
	int		i;
	char	*result;

	i = 0;
	if (ft_strlen(src) == 0)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!result)
		return (NULL);
	while (src[i])
	{
		result[i] = src[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_save_memory(char *str)
{
	int		i;
	char	*result;

	i = 0;
	while (str[i] != '\n' && str[i])
		i++;
	if (str[i] == '\n')
		i++;
	result = ft_strdup(str + i);
	free(str);
	return (result);
}
