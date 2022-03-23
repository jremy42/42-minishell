/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 16:10:37 by jremy             #+#    #+#             */
/*   Updated: 2022/03/23 09:09:59 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__bin_exit(char **key_val, t_msh *msh)
{
	int	i;

	i = 0;
	if (key_val[1] && key_val[2])
		return (__putstr_fd("exit: too many arguments\n", 2), 1);
	if (key_val[1])
	{
		while (key_val[1][i])
		{
			if (!__isdigit(key_val[1][i]) || key_val[1][0] == '-')
			{
				__putstr_fd("exit: numeric argument required\n", 2);
				msh->rv = 2;
				__exit(msh);
			}
			i++;
		}
		msh->rv = __atoi(key_val[1]);
		__exit(msh);
	}
	msh->rv = 0;
	__exit(msh);
	return (0);
}
