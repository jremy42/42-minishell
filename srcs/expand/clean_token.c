/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/06 12:51:57 by jremy             #+#    #+#             */
/*   Updated: 2022/04/06 15:15:18 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__clean_token(t_lexing **lexing)
{
	t_lexing	*iter;
	t_lexing	*last;

	iter = *lexing;
	last = NULL;
	while (iter)
	{
		if (iter->type == WORD && (iter->token)[0] == '\0')
		{
			//fprintf(stderr, "empty token spoted : [%s]\n", iter->token);

			if (last == NULL)
			{
				//fprintf(stderr, "Removing first\n");
				last = *lexing;
				*lexing = (*lexing)->next;
				free(last->token);
				free(last);
				iter = *lexing;
				last = NULL;
			}
			else
			{
				//fprintf(stderr, "Removing in middle\n");
				last->next = last->next->next;
				free(iter->token);
				free(iter);
				iter = last->next;
			}
		}
		else
		{
			//fprintf(stderr, "Skipping non-empty token : [%s] type [%d]\n", iter->token, iter->type);
			last = iter;
			iter = iter->next;
		}
	}
}
