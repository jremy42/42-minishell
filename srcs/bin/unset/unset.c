/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:29:51 by jremy             #+#    #+#             */
/*   Updated: 2022/03/29 19:06:00 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int __key_number(t_msh *msh, char *arg)
{
	int i;
	
	i = 0;
	while (msh->envp[i])
	{
		if (!__strncmp(msh->envp[i][0], arg, __strlen(arg)))
			return (i);
		i++;
	}
	return (-1);
}


static int	__check_valid_identifier_unset(char *key_val)
{
	int	i;
	int only_digit;

	i = 0;
	only_digit = 1;
	if (key_val && (key_val[0] == '=' || key_val[0] == '+'))
			return (print_error("unset", key_val, "not a valid identifier"), 0);
	while (key_val[i] && key_val[i] != '=')
	{
		if (key_val[i] == '+' && key_val[i + 1] == '=' && i > 0)
			return (1);
		if (!__isdigit(key_val[i]))
			only_digit = 0;
		if (!__isalnum(key_val[i]) && key_val[i] != '_')
			return (print_error("unset", key_val, "not a valid identifier"), 0);
		i++;
	}
	if (only_digit)
		return (print_error("unset", key_val, "not a valid identifier"), 0);
	return (1);
}

int	__unset(char **arg, t_msh *msh)
{
	int	i;
	int status;
	int key_number;

	status = __SUCCESS;
	i = 0;
	if (!arg[0])
		return (__SUCCESS);

	while (arg[i])
	{
		if (!__check_valid_identifier_unset(arg[i]))
			status = __FAIL;
		if (__key_number(msh, arg[i]) >= 0)
		{
			key_number = __key_number(msh, arg[i]);
			free(msh->envp[key_number][0]);
			msh->envp[key_number][0] = NULL;
			msh->envp[key_number][1][0] = '0';
			status = __SUCCESS;
		}
		i++;
	}
	return (status);
}
