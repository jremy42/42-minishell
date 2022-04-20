/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 11:20:24 by jremy             #+#    #+#             */
/*   Updated: 2022/01/24 17:07:08 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	if (!dst && !src)
		return (dst);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		*d++ = *s++;
		i++;
	}
	return (dst);
}

char	*ft_strjoingnl(char *dest, char *src)
{
	char	*result;
	int		size_dest;
	int		size_src;

	size_dest = ft_strlen(dest);
	size_src = ft_strlen(src);
	result = malloc(sizeof(char) * ((size_dest + size_src) + 1));
	if (!result)
		return (NULL);
	ft_memcpy(result, dest, size_dest);
	ft_memcpy(result + size_dest, src, size_src);
	free(dest);
	result[size_dest + size_src] = '\0';
	return (result);
}

int	ft_strchr(char *str, char tofind)
{
	int	i;

	i = 0;
	if (!str)
		return (1);
	while (str[i])
	{
		if (str[i] == tofind)
			return (-1);
		i++;
	}
	return (1);
}

char	*ft_read_file(int fd, char *memory)
{
	char	buff[BUFFER_SIZE + 1];
	int		r;

	r = 1;
	while (ft_strchr(memory, '\n') > 0 && (r > 0))
	{
		r = read(fd, buff, BUFFER_SIZE);
		if (r <= 0)
			break ;
		buff[r] = '\0';
		memory = ft_strjoingnl(memory, buff);
		if (!memory)
		{
			free(memory);
			return (NULL);
		}
	}
	return (memory);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*memory = NULL;

	if (fd == -1 && memory)
	{
		free(memory);
		memory = NULL;
	}
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	memory = ft_read_file(fd, memory);
	if (ft_strlen(memory) > 0)
	{
		line = ft_get_line(memory);
		memory = ft_save_memory(memory);
		return (line);
	}
	return (NULL);
}
