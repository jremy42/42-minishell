/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_basic_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:15:37 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 15:04:21 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

int	__basic_env(void)
{
	char	renv[BUFFER_SIZE]; 
	char	true_env[BUFFER_SIZE];
	t_pipe	std;
	int fd;
		
	system("env > env.txt");
	std.stdin_cpy = -1;
	std.stdout_cpy = -1;
	__init_get(&std);
	__env((char **)g_env);
	__read_std(renv, &std);
	__close_std(&std);
	fd = open("env.txt", O_RDONLY, 00644);
	bzero((void *)true_env, BUFFER_SIZE);
	read(fd, true_env, BUFFER_SIZE);
	if ((strcmp(renv, true_env) != 0))
		return (-1);
	else
		return (0);
}