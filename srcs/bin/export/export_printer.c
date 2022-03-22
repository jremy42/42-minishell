/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_printer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 09:36:33 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/22 09:39:04 by fle-blay         ###   ########.fr       */
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
		if (envp[i][1])
		{
			smallest = envp[i][0];
			break ;
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

static char	*get_biggest_exported_value(char ***envp)
{
	int		i;
	char	*biggest;

	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
		{
			biggest = envp[i][0];
			break ;
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

static char	*get_smallest_sup_value(char *old_small, char ***envp)
{
	char	*ret;
	int		i;

	ret = get_biggest_exported_value(envp);
	i = 0;
	while (envp[i])
	{
		if (envp[i][1])
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
	write(1, curr_printed, __strchr(curr_printed, '=') - curr_printed + 1);
	write(1, "\"", 1);
	write(1, __strchr(curr_printed, '=') + 1,
		__strlen(__strchr(curr_printed, '=')) - 1);
	write(1, "\"\n", 2);
}

int	print_exported_values(char ***envp)
{
	int		i;
	int		max;
	char	*old_printed;
	char	*curr_printed;

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
