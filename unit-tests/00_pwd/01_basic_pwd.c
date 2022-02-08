/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_basic_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:15:37 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 11:15:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

int	__basic_pwd(void)
{
	char	rpwd[BUFFER_SIZE];
	t_pipe	std;
	
	std.stdin_cpy = -1;
	std.stdout_cpy = -1;
	__init_get(&std);
	__pwd(1);
	__read_std(rpwd, &std);
	__close_std(&std);
	if ((strcmp(rpwd, "/mnt/nfs/homes/jremy/Desktop/minishell/unit-tests\n") != 0))
		return (-1);
	else
		return (0);
}