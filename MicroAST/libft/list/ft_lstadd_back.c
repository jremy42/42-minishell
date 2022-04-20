/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   __lstadd_back.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: jremy <jremy@student.42.fr>+#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2021/11/23 18:58:14 by jremy #+##+# */
/*   Updated: 2022/01/10 15:04:51 by jremy###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	__lstadd_back(t_list **alst, t_list *new)
{
	t_list	*nextlst;

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
