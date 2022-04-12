/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_seq.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:20:32 by jremy             #+#    #+#             */
/*   Updated: 2022/04/12 16:38:31 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*__find_path(char ***envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (__is_same_key(envp[i][0], "PATH" ))
			return (envp[i][0] + 5);
		i++;
	}
	return (NULL);
}

int	__find_max_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}

char	**__create_envp(char ***envp)
{
	int		size;
	int		i;
	int		j;
	char	**ret;

	size = 0;
	if (!envp)
		return (NULL);
	i = -1;
	while (envp[++i])
		size += (envp[i][1][0] == '1');
	i = 0;
	ret = malloc(sizeof(char *) * (size + 1));
	ret[size] = NULL;
	if (!ret)
		return (NULL);
	j = 0;
	while (envp[i])
	{
		if (envp[i][1][0] == '1')
			ret[j++] = envp[i][0];
		i++;
	}
	return (ret);
}

int	__init_seq(t_sequ *seq, char ***envp, t_cmd *cmd)
{
	seq->pipe[in] = -1;
	seq->pipe[out] = -1;
	seq->index = 0;
	seq->max_cmd = __find_max_cmd(cmd);
	if (envp && __find_path(envp))
	{
		seq->path = __split(__find_path(envp), ':');
		if (!seq->path)
			return (0);
	}
	else
		seq->path = NULL;
	seq->envp = __create_envp(envp);
	if (!seq->envp)
		return (free_split(seq->path), 0);
	return (1);
}
