/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/04/04 10:32:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_rv;

void	__signal(int signal)
{
	if (signal == 2)
	{	
		g_rv = 130;
		write(1, "\b\b", 2);
		write(1, "  ", 2);
		__putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	if (signal == 3)
	{
		write(1, "\b\b", 2);
		write(1, "  ", 2);
		write(1, "\b\b", 2);
		return ;
	}
}

void	__signal_treat(int signal)
{
	(void)signal;
	write(1, "\b\b\n", 3);
		g_rv = 130;
}

void	__signal_hd(int signal)
{
	(void)signal;
	close(0);
	g_rv = 1;
}
