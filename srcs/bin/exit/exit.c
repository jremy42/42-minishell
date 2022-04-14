/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 16:10:37 by jremy             #+#    #+#             */
/*   Updated: 2022/04/14 14:40:46 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	__isspace(char c)
{
	if (__strchr("\f\t\n\r\v ", c))
		return (1);
	return (0);
}

static int	__is_regular_space(char c)
{
	if (__strchr("\t ", c))
		return (1);
	return (0);
}

static int	__check_valid_number(char *value)
{
	int	i;

	i = 0;
	if (value)
	{
		while (value[i] && __isspace(value[i]))
			i++;
		if (value[i] && (value[i] == '-' || value[i] == '+') && value[i + 1])
			i++;
		while (value[i])
		{
			if (!__isdigit(value[i]))
			{
				while (value[i])
				{
					if (!__is_regular_space(value[i]))
						return (0);
					i++;
				}
				break ;
			}
			i++;
		}
	}
	return (1);
}

int	__bin_exit(char **key_val, t_msh *msh, t_cmd *cmd, int fork)
{
	if (key_val[1])
	{
		if (!__check_valid_number(key_val[1]))
		{
			if (isatty(0) && isatty(1))
				__putstr_fd("exit\n", 2);
			print_error("exit", key_val[1], "numeric argument required");
			msh->rv = 2;
			__cmd_node_list_clear(cmd);
			__exit(msh);
		}
		if (!__atol(key_val[1], &msh->rv))
		{
			print_error("exit", key_val[1], "numeric argument required");
			msh->rv = 2;
		}
		if (key_val[1] && key_val[2])
			return (__putstr_fd("exit\nexit: too many arguments\n", 2), 1);
	}
	if (fork)
		return (msh->rv);
	__cmd_node_list_clear(cmd);
	__putstr_fd("exit\n", 2);
	__exit(msh);
	return (0);
}
