/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/01 10:04:44 by jremy             #+#    #+#             */
/*   Updated: 2022/04/01 10:41:48 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*__get_prompt(int rv)
{
	char	path[PATH_MAX];
	char	*prompt;

	__bzero(path, PATH_MAX);
	if (rv == 0)
		prompt = __strdup(BOLDGREEN"➜  "RESET BOLDCYAN);
	else
		prompt = __strdup(BOLDRED"➜  "RESET BOLDCYAN);
	if (!prompt)
		return (NULL);
	if (getcwd(path, PATH_MAX))
	{
		prompt = __strjoin2(prompt,
				path + __get_pos_last_dir(path), RESET BOLDYELLOW "  ~  "RESET);
		if (!prompt)
			return (NULL);
	}
	else
		prompt = __strjoin(prompt, RESET);
	return (prompt);
}

void	__new_prompt(int rv)
{
	char	*prompt;

	if (isatty(2))
	{
		prompt = __get_prompt(rv);
		if (!prompt)
		{
			print_error("prompt", "Malloc error", NULL);
			return ;
		}
		__putstr_fd(prompt, 2);
		free(prompt);
	}
}
