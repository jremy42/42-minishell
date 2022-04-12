/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 10:04:49 by fle-blay          #+#    #+#             */
/*   Updated: 2022/04/12 10:27:04 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__strlen_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=' && str[i] != '+')
		i++;
	return (i);
}

int	__is_same_key(char *s1, char *s2)
{
	int	s1_len;
	int	s2_len;
	int	s1_in_s2;
	int	s2_in_s1;

	if (!s1 || !s2)
		return (0);
	s1_len = __strlen_equal(s1);
	s2_len = __strlen_equal(s2);
	s1_in_s2 = (__strncmp(s1, s2, s1_len) == 0);
	s2_in_s1 = (__strncmp(s1, s2, s2_len) == 0);
	if (s1_in_s2 && s2_in_s1)
		return (1);
	return (0);
}

int	modify_status_key_val(t_msh *msh, char *key_val)
{
	int		i;

	i = 0;
	while (msh->envp[i])
	{
		if (msh->envp[i][1][0] != '0'
		&& __is_same_key(msh->envp[i][0], key_val))
			break ;
		i++;
	}
	if (!msh->envp[i])
		return (add_key_val(msh, key_val, get_envp_size(msh)));
	free(msh->envp[i][1]);
	msh->envp[i][1] = __strdup("1");
	if (!msh->envp[i][1])
		return (__MALLOC);
	return (__SUCCESS);
}

char	*get_value(t_msh *msh, char *key)
{
	int		i;

	i = 0;
	while (msh->envp[i])
	{
		if (__is_same_key(msh->envp[i][0], key))
			break ;
		i++;
	}
	if (!msh->envp[i])
		return (NULL);
	if (__strchr(msh->envp[i][0], '='))
		return (__strchr(msh->envp[i][0], '=') + 1);
	return (NULL);
}

int	get_number_exported_values(char ***envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (envp[i])
	{
		if (envp[i][1][0] != '0')
			count++;
		i++;
	}
	return (count);
}
