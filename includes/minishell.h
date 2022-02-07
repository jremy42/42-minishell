/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 11:08:29 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 15:21:22 by jremy            ###   ########.fr       */
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
