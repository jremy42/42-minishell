/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstaddsort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 16:24:35 by jremy             #+#    #+#             */
/*   Updated: 2022/04/07 17:57:08 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	__lstaddsort(t_list **lst, t_list *new_elem,
		int (*f)(const char *, const char *))
{
	t_list	*previous;
	t_list	*index;

	previous = *lst;
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
			{
				new_elem->next = index;
				previous->next = new_elem;
				return ;
			}	
			previous = index;
			index = index->next;
		}
		if (f(index->content, new_elem->content) >= 0)
		{
			new_elem->next = index;
			previous->next = new_elem;
		}
		else
			index->next = new_elem;
	}
}
