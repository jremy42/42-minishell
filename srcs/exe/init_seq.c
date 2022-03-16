/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_seq.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/10 16:20:32 by jremy             #+#    #+#             */
/*   Updated: 2022/03/16 17:10:09 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static char	*__find_path(char ***envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (__strncmp(envp[i][0], "PATH", 4) == 0)
			return (envp[i][0] + 5);
		i++;
	}
	return (NULL);
}

int __find_max_cmd(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd)
	{
		cmd = cmd->next;
		i++;
	}
	return (i);
}

static char **__create_envp(char ***envp)
{
	int size;
	int i;
	char **ret;

	i = 0;
	size = 0;
	while (envp[size])
		size++;
	ret = malloc(sizeof(char *) * size);
	if (!ret)
		return (NULL);
	while (i < size)
	{
		ret[i] = envp[i][0];
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
	if (__find_path(envp))
	{
		seq->path = __split(__find_path(envp), ':');
		if (!seq->path)
			return (0);
	}
	seq->envp = __create_envp(envp);
	if (!seq->envp)
		return (0);
	return (1);
}

int D_print_seq(t_sequ *seq)
{
	int i;

	i = 0;
	DEBUG && printf("max cmd = %d\n", seq->max_cmd);
	while (seq->path[i])
	{
		printf("path = %s\n", seq->path[i]);
		i++;
	}
	return (1);
}
