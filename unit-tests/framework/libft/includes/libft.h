/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 15:17:36 by jremy             #+#    #+#             */
/*   Updated: 2022/01/19 09:19:44 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int		__isalpha(int c);
int		__isdigit(int c);
int		__isalnum(int c);
int		__isascii(int c);
int		__isprint(int c);
//size_t	__strlen(const char *str);
void	*__memset(void *pt, int value, size_t count);
void	__bzero(void *pt, int count);
void	*__memcpy(void *dst, const void *src, size_t n);
void	*__memmove(void *dst, const void *src, size_t size);
size_t	__strlcpy(char *dst, const char *src, size_t size);
size_t	__strlcat(char *dst, const char *src, size_t n);
int		__toupper(int c);
int		__tolower(int c);
char	*__strchr(const char *s1, int find);
char	*__strrchr(const char *s1, int find);
int		__strncmp(const char *s1, const char *s2, size_t size);
void	*__memchr(const void *buffer, int c, size_t count);
int		__memcmp(const void *buffer1, const void *buffer2, size_t n);
char	*__strnstr(const char *str, const char *find, size_t len);
int		__atoi(const char *nb);
void	*__calloc(size_t elementCount, size_t elementSize);
char	*__strdup(const char *src);
char	*__substr(char const *s, unsigned int start, size_t len);
char	*__strjoin(char const *s1, char const *s2);
char	*__strtrim(char const *s1, char const *set);
char	**__split(char const *s, char c);
char	*__itoa(int n);
char	*__strmapi(char const *s, char (*f)(unsigned int, char));
void	__striteri(char *s, void (*f)(unsigned int, char *));
void	__putchar_fd(char c, int fd);
void	__putstr_fd(char const *s, int fd);
void	__putendl_fd(char const *s, int fd);
void	__putnbr_fd(int n, int fd);
char	*get_next_line(int fd);
int		ft_strchr(char *str, char tofind);
int		ft_strlen(char *str);
char	*ft_strjoingnl(char *dest, char *src);
char	*ft_get_line(char *str);
int		ft_size_line(char *str);
char	*ft_save_memory(char *str);
char	*ft_strdup(char *src);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_read_file(int fd, char *memory);
#endif
