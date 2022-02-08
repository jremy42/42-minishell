/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 10:08:30 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 15:06:10 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

int	__loader(t_utest **alst, char *fun, char *name, int (*f)(void))
{
	t_utest	*utest;

	if (*alst == NULL)
	{
		utest = __newutest(fun, name, f);
		if (!utest)
			return (_FAIL);
		*alst = utest;
		return (_SUCCESS);
	}
	else
	{
		utest = __newutest(fun, name, f);
		if (!utest)
			return (_FAIL);
		__add_back(alst, utest);
		return (_SUCCESS);
	}
}
