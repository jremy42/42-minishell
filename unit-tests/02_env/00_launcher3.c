/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_launcher3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:04:29 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 14:51:28 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"
#include "tests.h"

int	ft02_launcher(t_score *score)
{
	t_utest	*testlist;
	int		res;

	res = 0;
	testlist = NULL;
	__loader(&testlist, "ENV", "basic", &__basic_env);
	__launcher(testlist, score);
	return (res);
}
