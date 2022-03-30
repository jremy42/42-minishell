/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_printer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 09:36:33 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/30 10:20:23 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_smallest_exported_value(char ***envp)
{
	int		i;
	char	*smallest;

	i = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0' && envp[i][0])
		{
			smallest = envp[i][0];
			break ;
		}
		i++;
	}
	i = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0' && envp[i][0])
		{
			if (__strcmp(smallest, envp[i][0]) > 0)
				smallest = envp[i][0];
		}
		i++;
	}
	return (smallest);
}

static char	*get_biggest_exported_value(char ***envp)
{
	int		i;
	char	*biggest;

	i = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0')
		{
			biggest = envp[i][0];
			break ;
		}
		i++;
	}
	i = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0' && envp[i][0])
		{
			if (__strcmp(biggest, envp[i][0]) < 0)
				biggest = envp[i][0];
		}
		i++;
	}
	return (biggest);
}

static char	*get_smallest_sup_value(char *old_small, char ***envp)
{
	char	*ret;
	int		i;

	ret = get_biggest_exported_value(envp);
	i = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0' && envp[i][0])
		{
			if (__strcmp(old_small, envp[i][0]) < 0)
				if (__strcmp(ret, envp[i][0]) > 0)
					ret = envp[i][0];
		}
		i++;
	}
	return (ret);
}

static void	__get_printed_export(char *curr_printed)
{
	write(1, "export ", 7);
	if (__strchr(curr_printed, '='))
	{
		write(1, curr_printed, __strchr(curr_printed, '=') - curr_printed + 1);
		write(1, "\"", 1);
		write(1, __strchr(curr_printed, '=') + 1,
			__strlen(__strchr(curr_printed, '=')) - 1);
		write(1, "\"\n", 2);
	}
	else
		__putendl_fd(curr_printed, 1);
}

int	print_exported_values(char ***envp)
{
	int		i;
	int		max;
	char	*old_printed;
	char	*curr_printed;

	if (!envp || !envp[0])
		return (0);
	i = 0;
	max = get_number_exported_values(envp);
	if (max == 0)
		return (0);
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
	return (0);
}
