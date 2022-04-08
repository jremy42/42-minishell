/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddsort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 16:24:35 by jremy             #+#    #+#             */
/*   Updated: 2022/04/08 09:44:07 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	__swap_lst(t_list *new_elem, t_list *previous,
	t_list *index, t_list **lst)
{
	new_elem->next = index;
	if (previous)
		previous->next = new_elem;
	else
		*lst = new_elem;
}

void	__lstaddsort(t_list **lst, t_list *new_elem,
		int (*f)(const char *, const char *))
{
	t_list	*previous;
	t_list	*index;

	previous = NULL;
	if (!(f))
		return ;
	if (!(*lst))
		*lst = new_elem;
	else
	{
		index = *lst;
		while (index->next)
		{
			if (f(index->content, new_elem->content) >= 0)
				return (__swap_lst(new_elem, previous, index, lst));
			previous = index;
			index = index->next;
		}
		if (f(index->content, new_elem->content) >= 0)
			return (__swap_lst(new_elem, previous, index, lst));
		else
			index->next = new_elem;
	}
}
