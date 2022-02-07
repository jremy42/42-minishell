/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   __lstiter.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: jremy <jremy@student.42.fr>+#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2021/11/23 19:01:27 by jremy #+##+# */
/*   Updated: 2022/01/10 15:04:51 by jremy###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

void	__lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*nextlst;

	if (f)
	{
		while (lst != NULL)
		{
			nextlst = lst->next;
			f(lst->content);
			lst = nextlst;
		}
	}
}
