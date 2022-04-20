/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 16:31:34 by jremy             #+#    #+#             */
/*   Updated: 2022/02/08 17:03:58 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
