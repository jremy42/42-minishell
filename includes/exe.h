/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:32:24 by jremy             #+#    #+#             */
/*   Updated: 2022/03/11 12:23:20 by jremy            ###   ########.fr       */
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
t_redirect   *__create_new_redirect(t_lexing *lexing);
void __redirect_add_back(t_redirect **alst, t_redirect *new);
int __add_redirect(t_cmd *cmd, t_lexing *lexing);
void	__redirect_list_clear(t_redirect *start);
void __exit_child(t_sequ *seq, t_cmd *cmd, int errno_copy, int error);
int __handle_redirect(t_sequ *seq, t_cmd *cmd);
int __handle_redirect_builtin(t_cmd *cmd);
int __find_max_cmd(t_cmd *cmd);
#endif
