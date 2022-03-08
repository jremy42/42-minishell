/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 17:32:24 by jremy             #+#    #+#             */
/*   Updated: 2022/03/08 18:36:00 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXE_H
# define EXE_H

# define _HD 1
typedef enum s_pipe
{
	in;
	out
}	t_pipe;

typedef struct s_sequ
{
	int		std_in;
	int		std_out;
	int		max_cmd;
	int		index;
	int		pipe[2];
	char	*path;
	char	**av;
    char    **envp;
}	t_sequ;

#endif