/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_spliting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:10:26 by jremy             #+#    #+#             */
/*   Updated: 2022/04/01 17:38:00 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	__split_token(t_lexing *lexing, t_msh *msh)
{
	t_list		*start;
	t_list		*save;
	char		*to_tokenize;

	start = NULL;
	to_tokenize = __strtrim(lexing->token, " \f\t\r\v");
	if (!to_tokenize)
		return (0);
	free(lexing->token);
	lexing->token = to_tokenize;
	if (!__tokenize(lexing->token, &start, msh))
		return (0);
	save = start;
	while (start)
	{
		if (!__insert_token(lexing, (char *)(start->content), 0, NULL))
			return (__lstclear(&save, free), 0);
		start = start->next;
	}
	__lstclear(&save, free);
	__insert_token(NULL, NULL, 1, NULL);
	return (1);
}

int	__field_spliting_token(t_lexing *lexing, t_msh *msh)
{
	int	before_type;

	before_type = 0;
	while (lexing)
	{
		if (lexing->type == WORD && before_type != HERE_DOC)
		{
			fprintf(stderr,"lexing envoyer = [%s]\n", lexing->token);
			if (!__split_token(lexing, msh))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		before_type = lexing->type;
		lexing = lexing->next;
	}
	return (1);
}
