/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   __lstclear.c  :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: jremy <jremy@student.42.fr>+#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2021/11/23 19:00:21 by jremy #+##+# */
/*   Updated: 2022/01/10 15:04:51 by jremy###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	__lstclear(t_list **alst, void (*del)(void *))
{
	t_list	*tmp;

	if (!alst)
		return ;
	while (*alst != NULL)
	{
		tmp = (*alst)->next;
		del((*alst)->content);
		free(*alst);
		*alst = tmp;
	}
}
