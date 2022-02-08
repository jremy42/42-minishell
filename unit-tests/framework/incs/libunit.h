/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libunit.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 13:00:25 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 17:51:16 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBUNIT_H
# define LIBUNIT_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/includes/libft.h"

// 📶 Signals transcriptor
# define _SUCCESS 0
# define _FAIL -1
# define _KO 255
# define _SEGFAULT 11
# define _SEGBUS 7
# define _PID_ERROR 666
# define _LEAK 69
// color

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

// 🖌️ Colors  
# define _MSG_OK  "\033[1m\033[32m[OK]\n \033[0m"
# define _MSG_KO "\033[1m\033[31m[KO] \n\033[0m"
# define _MSG_SEG "\033[1m\033[33m[SIGSEGV] \n\033[0m"
# define _MSG_BUS "\033[1m\033[33m[SIGBUS] \n\033[0m"
# define _MSG_UKN "\033[1m\033[33m[UKN] \n\033[0m"
# define _MSG_LEAK "\033[1m\033[33m[LEAKS] \n\033[0m"


# define BUFFER_SIZE 1000

// Typedef
typedef struct s_pipe
{
	int		stdout_cpy;
	int		stdin_cpy;
	int		pipe_fds[2];
}	t_pipe;

typedef struct s_ret
{
	int		ret1;
	int		ret2;
}	t_ret;

typedef struct s_print
{
	int				d;
	char			c;
	char			*str;
	void			*p;
	unsigned int	u;
}	t_print;

// 📦 Typedef / Struct
typedef struct s_utest
{
	char			*name;
	char			*fun;
	int				(*f)(void);
	struct s_utest	*next;
}	t_utest;

typedef struct s_score
{
	int	ok;
	int	total;
}	t_score;

// 📜 utils_list
t_utest		*__newutest(char *fun, char *name, int (*f)(void));
void		__add_front(t_utest **alst, t_utest *new);
void		__add_back(t_utest **alst, t_utest *new);
void		__lstclear(t_utest *alst);

// 📚 Libs
void		__putstr(char *str);
void		__putnbr(int n);
int			__print_score(int test_cnt, int cnt);
void		__print_global_summary(int ok, int total);

// 🚀 _launcher 
int			__loader(t_utest **alst, char *fun, char *name, int (*f)(void));
int			__launcher(t_utest *alst, t_score *score);

// 🐛 _debug
void		__print_utest(t_utest *utest);

int		__init_get(t_pipe *std);
void	__read_std(char *buff, t_pipe *std);
void	__close_std(t_pipe *std);
void	__start();
void	__log_name(char *fun, char *name);
void	__log_test(char *test, char *att, char* ret);
int		__leaks(int fd);

#endif