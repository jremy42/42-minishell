/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 10:21:11 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/17 13:10:46 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_key(t_msh *msh, char *key)
{
	int		i;

	i = 0;
	while (msh->envp[i])
	{
		if (!__strncmp(msh->envp[i][0], key, __strlen(key)))
			break ;
		i++;	
	}
	if (!msh->envp[i])
	{
		__putstr_fd(key, 2);
		__putstr_fd("not set\n", 2);
		return (NULL);
	}
	return (__strchr(msh->envp[i][0], '=') + 1);
}

int	print_exported_values(char ***envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			write(1, "export ", 7);
			write(1, envp[i][0], __strchr(envp[i][0], '=') - envp[i][0] + 1);
			write(1, "\"", 1);
			write(1, __strchr(envp[i][0], '=') + 1, __strlen(__strchr(envp[i][0], '=')) - 1 );
			write(1, "\"\n", 2);
		}
		i++;
	}
	return (1);
}


int	__check_valid_identifier(char *key_val)
{
	int	i;

	i = 0;
	while(key_val[i] && key_val[i]!= '=')
	{
		if (key_val[i]== '+' && key_val[i + 1] == '=')
			return (1);
		if (!__isalnum(key_val[i]) && key_val[i]!= '_')
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

	i = -1;
	status = __SUCCESS;	
	if (!key_val[++i])
	{
		status = print_exported_values(msh->envp);
		return (status);
	}
	i = -1;
	while (status == __SUCCESS && key_val[++i])
	{
		if (__strchr(key_val[i], '='))
		{
			if (!__check_valid_identifier(key_val[i]))
				return (__FAIL);
			if (!key_exist(msh, key_val[i]))
				status = add_key_val(msh, key_val[i], get_envp_size(msh));
			else
				status = update_key_val(msh, key_val[i]);
		}
		else
			if (!__check_valid_identifier(key_val[i]))
				return (__FAIL);
			status = modify_status_key_val(msh, key_val[i]);
	}
	return (status);
}
