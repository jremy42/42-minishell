/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/04 12:56:27 by jremy             #+#    #+#             */
/*   Updated: 2022/04/05 16:43:45 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*__create_path(char *split_cd, char *new_path)
{
	if (!split_cd || !new_path)
		return (NULL);
	if (__strlen(split_cd) > 0 && split_cd[__strlen(split_cd) - 1] != '/')
		return (__create_path_and_cmd(split_cd, new_path));
	return (__old_strjoin(split_cd, new_path));
}

int	__try_path_cdpath(char *new_path, t_msh *msh, char **final_path)
{
	int			i;
	struct stat	buff;
	char		**split_cd;

	split_cd = __split(get_key(msh, "CDPATH"), ':');
	i = 0;
	if (!split_cd)
		return (__putstr_fd("MALLOC ERROR\n", 2), 2);
	while (split_cd[i])
	{
		*final_path = __create_path(split_cd[i], new_path);
		if (!*final_path)
			return (free_split(split_cd), __putstr_fd("MALLOC ERROR\n", 2), 2);
		stat(*final_path, &buff);
		if (access(*final_path, F_OK) == 0 && S_ISDIR(buff.st_mode))
			return (free_split(split_cd), 1);
		i++;
		free(*final_path);
		*final_path = NULL;
	}
	return (free_split(split_cd), 0);
}

int	__try_change_dir(char *final_path, char *new_path, t_msh *msh, int verbose)
{
	int	ret;

	if (!__access_dir(final_path, new_path))
		return (free(final_path), __FAIL);
	else
	{
		if (update_oldpwd(msh) == __MALLOC)
			return (free(final_path), __MALLOC);
		chdir(final_path);
		free(final_path);
	}
	ret = update_pwd(msh);
	if (verbose)
		__pwd(1);
	return (ret);
}
