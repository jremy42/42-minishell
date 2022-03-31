/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:21:42 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 10:53:48 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_rv;

void	__update_rv(t_msh *msh)
{
	if (g_rv > 0)
		msh->rv = g_rv;
	g_rv = 0;
}

void	__clean_inputs(char **inputs, t_msh *msh, char *arg)
{
	free_split(inputs);
	inputs = NULL;
	msh->all_input = NULL;
	free(arg);
}

int	__check_input(char *arg, char **to_tokenize, t_msh *msh)
{
	msh->syntax_error = 0;
	if (!__strcmp(arg, ""))
		return (0);
	*to_tokenize = __strtrim(arg, " \f\t\r\v");
	if (!*to_tokenize)
		return (__exit_error(msh, 3, "check input"));
	if (!__strcmp(*to_tokenize, ""))
		return (free(*to_tokenize), 0);
	return (1);
}

t_user_input	*__init_user_input_struct(t_user_input *ui)
{
	ui->token = NULL;
	ui->lexing = NULL;
	ui->parenthesis = NULL;
	return (ui);
}

int	__get_pos_last_dir(char *cwd)
{
	int	i;

	i = 0;
	while (cwd[i])
		i++;
	while (cwd[i] != '/' && i >= 0)
		i--;
	i++;
	return (i);
}
