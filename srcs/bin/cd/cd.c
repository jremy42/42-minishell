/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 14:13:35 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/25 12:51:31 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

int	__cd(char *new_path, t_msh *msh)
{
	if (!new_path)
	{
		update_oldpwd(msh);
		chdir(get_key(msh, "HOME"));
	}
	else if (new_path[0] == '-' && __strlen(new_path) == 1)
		chdir_previous(msh);
	else
	{ update_oldpwd(msh);
		if (new_path[0] == '/')
			chdir(new_path);
		else
			chdir_absolute_path(new_path);
	}
	update_pwd(msh);
	return (__SUCCESS);
}
