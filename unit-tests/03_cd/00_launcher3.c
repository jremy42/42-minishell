/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_launcher3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 10:04:29 by ngiroux           #+#    #+#             */
/*   Updated: 2022/02/08 17:25:00 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"
#include "tests.h"

int	ft03_launcher(t_score *score)
{
	t_utest	*testlist;
	int		res;

	res = 0;
	testlist = NULL;
	__loader(&testlist, "CD", "basic absolute path", &__basic_cd);
	__loader(&testlist, "CD", "basic relative path", &__relative_cd);
	__launcher(testlist, score);
	return (res);
}
