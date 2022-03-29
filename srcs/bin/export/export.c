/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 10:06:28 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/29 18:39:41 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__check_valid_identifier(char *key_val)
{
	int	i;
	int only_digit;

	i = 0;
	only_digit = 1;
	if (key_val && (key_val[0] == '=' || key_val[0] == '+'))
			return (print_error("export", key_val, "not a valid identifier"), 0);
	while (key_val[i] && key_val[i] != '=')
	{
		if (key_val[i] == '+' && key_val[i + 1] == '=' && i > 0)
			return (1);
		if (!__isdigit(key_val[i]))
			only_digit = 0;
		if (!__isalnum(key_val[i]) && key_val[i] != '_')
			return (print_error("export", key_val, "not a valid identifier"), 0);
		i++;
	}
	if (only_digit)
		return (print_error("export", key_val, "not a valid identifier"), 0);
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
	while (status != __MALLOC && key_val[++i])
	{
		if (!__check_valid_identifier(key_val[i]))
			status = __FAIL;
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
