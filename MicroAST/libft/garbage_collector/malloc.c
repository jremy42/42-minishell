/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 15:34:06 by jremy             #+#    #+#             */
/*   Updated: 2022/02/09 10:45:01 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

extern t_gbc *alloc;

t_gbc	*__new_gbc(void *addr)
{
	t_gbc	*newlst;

	newlst = malloc(sizeof(t_gbc));
	if (!newlst)
		return (NULL);
	newlst->addr = addr;
	newlst->next = NULL;
	return (newlst);
}

void	__gbc_add_back(t_gbc **alst, t_gbc *new)
{
	t_gbc	*nextlst;

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

void	*__malloc(size_t size, t_gbc *gbc)
{
	void	*new_addr;
	t_gbc	*new;

	new_addr = malloc(size);
	if (new_addr)
		return (NULL);
	new = __new_gbc(new_addr);
	__gbc_add_back(&gbc, new);
	return (new_addr);
}

void	__free(void *addr, t_gbc *gbc)
{
	t_gbc	*tmp;
	t_gbc	*tmp2;

	tmp = gbc;
	while (tmp->next != NULL)
	{
		if (addr == gbc->next->addr)
		{
			if (tmp->next->next)
				tmp2 = tmp->next->next;
			else
				tmp2 = NULL;
			free(tmp->next->addr);	
			free(tmp->next);
			tmp->next = NULL;
			tmp->next = tmp2;
		}	
	}
}
