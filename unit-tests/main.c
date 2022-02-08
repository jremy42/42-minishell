/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:01:04 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 10:27:53 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include "libunit.h"
#include "minishell.h"

int	main(void)
{
	t_score	score;

	score.ok = 0;
	score.total = 0;
	__start();
	if (FT_TESTED[0] == '1')
		ft00_launcher(&score);
	if (FT_TESTED[0] == '1')
		ft01_launcher(&score);
	__print_global_summary(score.ok, score.total);
	if (score.ok == score.total)
		return (0);
	else
		return (-1);
}
