/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_spliting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:10:26 by jremy             #+#    #+#             */
/*   Updated: 2022/03/22 15:29:43 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




int	__split_token(t_lexing *lexing, t_msh *msh)
{
	t_list		*start;
	t_list		*save;
	start = NULL;
	
	__tokenize(lexing->token, &start, msh);
	save = start;
	while (start)	
	{
		if (!__insert_token(lexing, (char * )(start->content), 0, NULL))
			return (__lstclear(&save, free), 0);
		start = start->next;
	}
	__lstclear(&save, free);
	__insert_token(NULL, NULL, 1, NULL);
	return (1);
}

int __field_spliting_token(t_lexing *lexing, t_msh *msh)
{
	while(lexing)
	{
		if(lexing->type == WORD)
		{
			DEBUG && fprintf(stderr, "new_token before field spliting: [%s]\n", lexing->token);
			if(!__split_token(lexing, msh))
				return (__putendl_fd("Malloc error", 2), 0);
			DEBUG && fprintf(stderr, "new_token after field spliting (first): [%s]\n", lexing->token);
		}
		lexing = lexing->next;
	}
	return (1);
}