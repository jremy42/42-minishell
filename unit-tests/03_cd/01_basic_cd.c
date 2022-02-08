/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_basic_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:18:59 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/08 17:29:42 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

int	__basic_cd(void)
{
	char	rpwd[BUFFER_SIZE]; 
	t_pipe	std;

	std.stdin_cpy = -1;
	std.stdout_cpy = -1;
	__init_get(&std);
	__cd("/dev");
	__pwd(1);
	__read_std(rpwd, &std);
	__close_std(&std);
	if ((strcmp(rpwd, "/dev\n") != 0))
		return (-1);
	else
		return (0);
}
