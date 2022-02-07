/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 15:43:59 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 16:05:25 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINTF_H
# define PRINTF_H

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"
# include <stdarg.h>

typedef struct s_flags
{
	char	type;
	int		minus;
	int		dot;
	int		ndot;
	int		blank;
	int		hash;
	int		zero;
	int		plus;
	int		widht;
	int		nullbit;
	int		negatif;
	size_t	value;
	char	*content;
}	t_flags;

int		__count_flags(char *str);
int		__mfw_size(t_flags *flags);
int		__is_flag(char flag);
int		__valide_type(char type);
int		__parsing_flags(char *src, t_flags *flags);
int		__printf(const char *str, ...);
void	__print_struct(t_flags flags);
void	__return_u_int(unsigned int d, t_flags *flags);
void	__return_hexa(unsigned int x, t_flags *flags, char type);
void	__return_u_int(unsigned int d, t_flags *flags);
void	__putchar(int c);
char	*__str_convert(unsigned long long nb, char *base_to);
int		__parsing_type(char type, t_flags *flags, va_list args);
int		__treat_dot(t_flags *flags);
int		__print(t_flags *flags);
int		__treat_mfw(int mfw, t_flags *flags);
int		__treat_mfwr(int mfw, t_flags *flags);
void	__return_percent(t_flags *flags);
int		__next_i(char *src);
int		__treat_zero(int mfw, t_flags *flags);
void	__putstr(char const *s);
#endif
