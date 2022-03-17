/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 10:21:11 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/17 14:47:26by jremy            ###   ########.fr       */
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

int	get_number_exported_values(char ***envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (envp[i])
	{
		if (envp[i][1])
			count++;
		i++;
	}
	return (count);
}

char	*get_smallest_exported_value(char ***envp)
{
	int	i;
	char *smallest;

	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			smallest = envp[i][0];
			break;
		}
		i++;
	}
	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			if (__strcmp(smallest, envp[i][0]) > 0)
				smallest = envp[i][0];
		}
		i++;
	}
	return (smallest);
}

char	*get_biggest_exported_value(char ***envp)
{
	int	i;
	char *biggest;

	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			biggest = envp[i][0];
			break;
		}
		i++;
	}
	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			if (__strcmp(biggest, envp[i][0]) < 0)
				biggest = envp[i][0];
		}
		i++;
	}
	return (biggest);
}


char *get_smallest_sup_value(char *old_small, char ***envp)
{
	char *ret;
	int	i;

	ret = get_biggest_exported_value(envp);

	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			if (__strcmp(old_small, envp[i][0]) < 0)
				if(__strcmp(ret, envp[i][0]) > 0)
					ret = envp[i][0];
		}
		i++;
	}
	return (ret);
}

void __get_printed_export(char *curr_printed)
{
	write(1, "export ", 7);
	write(1, curr_printed, __strchr(curr_printed, '=') - curr_printed + 1);
	write(1, "\"", 1);
	write(1, __strchr(curr_printed, '=') + 1, __strlen(__strchr(curr_printed, '=')) - 1 );
	write(1, "\"\n", 2);
}

int	print_exported_values(char ***envp)
{
	int	i;
	int max;
	char *old_printed;
	char *curr_printed;

	i = 0;
	max = get_number_exported_values(envp);
	old_printed = get_smallest_exported_value(envp);
	__get_printed_export(old_printed);
	while (max - 1)
	{
		curr_printed = get_smallest_sup_value(old_printed, envp);
		__get_printed_export(curr_printed);
		old_printed = curr_printed; 
		max--;
		i = 0;
	}
	return (1);
}


int	__check_valid_identifier(char *key_val)
{
	int	i;

	i = 0;
	while(key_val[i] && key_val[i]!= '=')
	{
		if (key_val[i]== '+' && key_val[i + 1] == '=' && i > 0)
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
			else if(__strstr(key_val[i],"+="))
				status = join_key_val(msh, key_val[i]);
			else
				status = update_key_val(msh, key_val[i]);
		}
		else
		{
			if (!__check_valid_identifier(key_val[i]))
				return (__FAIL);
			status = modify_status_key_val(msh, key_val[i]);
		}
	}
	return (status);
}
