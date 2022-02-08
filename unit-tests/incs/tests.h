/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 17:31:19 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 11:44:23 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

# include <stdlib.h>
# include <libunit.h>
# include "../includes/minishell.h"

int	ft00_launcher(t_score *score);
int	ft01_launcher(t_score *score);
int	__basic_echo(void);
int	__basic_pwd(void);
int	__flag_echo(void);
int	__multi_flag_echo(void);
int	__NULL_f_echo(void);
int	__NULL_echo(void);
int	__invalid_flag_echo(void);
#endif
