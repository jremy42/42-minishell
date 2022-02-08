/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_launcher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:04:29 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 11:15:29 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"
#include "tests.h"

int	ft00_launcher(t_score *score)
{
	t_utest	*testlist;
	int		res;

	testlist = NULL;
	if (FT00_TEST[0] == '1')
		__loader(&testlist, "PWD", "basic", &__basic_pwd);	
	res = __launcher(testlist, score);
	return (res);
}
