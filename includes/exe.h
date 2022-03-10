/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:32:24 by jremy             #+#    #+#             */
/*   Updated: 2022/03/10 16:43:47 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_H
# define EXE_H

# define _HD 1

enum {in, out};

typedef struct s_sequ
{
	int		max_cmd;
	int		index;
	int		pipe[2];
	char	**path;
    char    **envp;
}	t_sequ;

void	__cmd_list_clear(t_cmd *start);
t_cmd *create_cmd_list(t_lexing *lexing, t_msh *msh);
void execute_child(t_sequ *seq, t_cmd *cmd);
int	__init_seq(t_sequ *seq, char ***envp, t_cmd *cmd);
int __launcher_fork(t_sequ *seq, t_cmd *cmd);
void	__exec_builtin(char **arg, t_msh *msh);
int __is_builtin(char **arg);
#endif
