/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_launcher2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:04:29 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 14:06:22 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"
#include "tests.h"

int	ft01_launcher(t_score *score)
{
	t_utest	*testlist;
	int		res;

	res = 0;
	testlist = NULL;
	__loader(&testlist, "ECHO", "basic", &__basic_echo);
	__loader(&testlist, "ECHO", "flag", &__flag_echo);
	__loader(&testlist, "ECHO", "multi f", &__multi_flag_echo);
	__loader(&testlist, "ECHO", "NULL", &__NULL_echo);
	//__loader(&testlist, "ECHO", "NULL_f", &__NULL_f_echo);
	__loader(&testlist, "ECHO", "invalid flag", &__invalid_flag_echo);
	__launcher(testlist, score);
	return (res);
}
