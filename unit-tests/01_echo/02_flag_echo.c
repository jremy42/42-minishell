/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_flag_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:15:37 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 11:28:38 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

int	__flag_echo(void)
{
	char 	**send;
	char	recho[BUFFER_SIZE]; 
	t_pipe	std;

	send = __split("echo -n ceci est un test", ' ');
	std.stdin_cpy = -1;
	std.stdout_cpy = -1;
	__init_get(&std);
	__echo(send, 1);
	__read_std(recho, &std);
	__close_std(&std);
	if ((strcmp(recho, "ceci est un test") != 0))
		return (-1);
	else
		return (0);
}