/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   __lstmap.c:+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: jremy <jremy@student.42.fr>+#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2021/11/24 10:37:44 by jremy #+##+# */
/*   Updated: 2022/01/10 15:04:51 by jremy###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

t_list	*__lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_elem;

	new_lst = NULL;
	if (!(lst) || !(f))
		return (NULL);
	new_elem = __lstnew(f(lst->content));
	if (!new_elem)
		return (NULL);
	new_lst = new_elem;
	lst = lst->next;
	while (lst)
	{
		new_elem = __lstnew(f(lst->content));
		if (!new_elem)
		{
			__lstclear(&new_lst, del);
			break ;
		}
		__lstadd_back(&new_lst, new_elem);
		lst = lst->next;
	}
	return (new_lst);
}
