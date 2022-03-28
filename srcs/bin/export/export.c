/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 10:06:28 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/28 10:07:08 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__check_valid_identifier(char *key_val)
{
	int	i;

	i = 0;
	while (key_val[i] && key_val[i] != '=')
	{
		if (key_val[i] == '+' && key_val[i + 1] == '=' && i > 0)
			return (1);
		if (!__isalnum(key_val[i]) && key_val[i] != '_')
		{
			__putstr_fd("Minishell : export : '", 2);
			__putstr_fd(key_val, 2);
			__putendl_fd("' not a valid identifier", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

int	__export(char **key_val, t_msh *msh)
{
	int	i;
	int	status;

	status = __SUCCESS;
	if (!key_val[0])
		return (print_exported_values(msh->envp));
	i = -1;
	while (status == __SUCCESS && key_val[++i])
	{
		if (!__check_valid_identifier(key_val[i]))
			return (__FAIL);
		if (__strchr(key_val[i], '='))
		{
			if (key_exist(msh, key_val[i]) == -1)
				status = add_key_val(msh, key_val[i], get_envp_size(msh));
			else if (__strstr(key_val[i], "+="))
				status = join_key_val(msh, key_val[i]);
			else
				status = update_key_val(msh, key_val[i]);
		}
		else
			status = modify_status_key_val(msh, key_val[i]);
	}
	return (status);
}
