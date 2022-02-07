/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:08:29 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 18:58:13 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

#define PATH_MAX 4096
#define __SUCCESS 0
#define __FAIL 1

typedef struct s_cmd
{
	char			*cmd;
	char			*flag;
	int				index;
	int				bin;
	int				fd;
	struct s_cmd	*next;
}	t_cmd;


/*
decouper en token / mots / operateur
cmd simple 
exe
fonction de gestion des fd
*/
//built-in
int __pwd(int fd);
int	__echo(char **str, int fd);

#endif

