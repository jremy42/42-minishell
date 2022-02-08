/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 10:02:55 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 17:40:07 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

t_utest	*__newutest(char *fun, char *name, int (*f)(void))
{
	t_utest	*newlst;

	newlst = malloc(sizeof(t_utest));
	if (!newlst)
		return (NULL);
	newlst->fun = fun;
	newlst->name = name;
	newlst->f = f;
	newlst->next = NULL;
	return (newlst);
}

void	__add_front(t_utest **alst, t_utest *new)
{
	if (!*alst)
		new->next = NULL;
	else
		new->next = (*alst);
	(*alst) = new;
}

void	__add_back(t_utest **alst, t_utest *new)
{
	t_utest	*nextlst;

	if (!(*alst))
		*alst = new;
	else
	{
		nextlst = (*alst);
		while (nextlst->next)
			nextlst = nextlst->next;
		nextlst->next = new;
	}
}

void	__lstclear(t_utest *alst)
{
	t_utest	*tmp;

	if (!alst)
		return ;
	while (alst != NULL)
	{
		tmp = (alst)->next;
		free(alst);
		alst = tmp;
	}
}
