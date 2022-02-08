/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:29:51 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 18:30:38 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int __unset(char** arg, char **env)
{
	int	i;

	i = 0;
	if (__strncmp(arg[0], "unset", 5) != 0)
		return (_FAIL);
	i++;
	while (arg[i] != NULL)
	{
		if (__get_env(arg, env) != NULL)
		{
			
		}
	}
}