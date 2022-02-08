/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 16:01:03 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 17:03:20 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# define BUFFER_SIZE 1

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_gbc
{
	void				*addr;
	struct s_gbc		*next;
}	t_gbc;

void	*__malloc(size_t size, t_gbc *gbc);
void	__free(void *addr, t_gbc *gbc);
int		__isalpha(int c);
int		__isdigit(int c);
int		__isalnum(int c);
int		__isascii(int c);
int		__isprint(int c);
size_t	__strlen(const char *str);
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
t_list	*__lstnew(void *content);
void	__lstadd_front(t_list **alst, t_list *new);
int		__lstsize(t_list *lst);
t_list	*__lstlast(t_list *lst);
void	__lstadd_back(t_list **alst, t_list *new);
void	__lstdelone(t_list *alst, void (*del)(void *));
void	__lstclear(t_list **alst, void (*del)(void *));
void	__lstiter(t_list *lst, void (*f)(void *));
t_list	*__lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
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
