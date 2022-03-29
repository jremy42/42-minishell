/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 16:10:37 by jremy             #+#    #+#             */
/*   Updated: 2022/03/29 16:35:17 by fred             ###   ########.fr       */
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

int	__bin_exit(char **key_val, t_msh *msh, t_cmd *cmd)
{
	int	i;

	i = 0;
	if (key_val[1] && key_val[2])
		return (__putstr_fd("exit: too many arguments\n", 2), 1);
	if (key_val[1])
	{
		while (key_val[1][i] && __isspace(key_val[1][i]))
			i++;
		if (key_val[1][i] && (key_val[1][i] == '-' || key_val[1][i] == '+') && key_val[1][i + 1])
			i++;
		while (key_val[1][i])
		{
			if (!__isdigit(key_val[1][i]))
			{
				while (key_val[1][i])
				{
					if (!__is_regular_space(key_val[1][i]))
						{
							print_error("exit", key_val[1], "numeric argument required"); 
							msh->rv = 2;
							__cmd_node_list_clear(cmd);
							__exit(msh);
						}
					i++;
				}
				break ;
			}
			i++;
		}
		msh->rv = __atoi(key_val[1]);
		__cmd_node_list_clear(cmd);
		__exit(msh);
	}
	//msh->rv = 0;
	__cmd_node_list_clear(cmd);
	__exit(msh);
	return (0);
}
