/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 10:21:11 by fle-blay          #+#    #+#             */
/*   Updated: 2022/02/09 14:14:27 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__export(char **key_val, t_msh *msh)
{
	int	i;
	int	status;

	i = -1;
	status = __SUCCESS;
	while (status == __SUCCESS && key_val[++i])
	{
		if (__strchr(key_val[i], '='))
		{
			if (!key_exist(msh, key_val[i]))
				status = add_key_val(msh, key_val[i], get_envp_size(msh));
			else
				status = update_key_val(msh, key_val[i]);
		}
		else
			status = modify_status_key_val(msh, key_val[i]);
	}
	return (status);
}
