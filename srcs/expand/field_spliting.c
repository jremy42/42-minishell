/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_spliting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 13:10:26 by jremy             #+#    #+#             */
/*   Updated: 2022/04/07 10:27:31 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
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
*/

int	__split_token(t_lexing *lexing, t_msh *msh)
{
	char		*to_field_split;
	char		**field_split;
	int			i;
	t_lexing	*true_end;

	(void)msh;
	to_field_split = __strtrim(lexing->token, " \f\t\r\v");
	if (!to_field_split)
		return (0);
	field_split = __split_unquoted_charset(to_field_split, "\n ");
	free(to_field_split);
	if (!field_split)
		return (0);
	true_end = lexing->next;
	i = 0;
	while (field_split[i])
	{
		if (!__insert_token(lexing, field_split[i], 0, true_end))
			return (free_split(field_split), 0);
		i++;
	}
	free_split(field_split);
	__insert_token(NULL, NULL, 1, NULL);
	return (1);
}
//"ab"$var
int	__field_spliting_token(t_lexing *lexing, t_msh *msh)
{
	int	before_type;

	before_type = 0;
	while (lexing)
	{
		//fprintf(stderr, "lexing->token = [%s]\n", lexing->token);
		/*
		if (lexing->type == WORD && before_type != HERE_DOC
			&& lexing->token[0] != '"'
			&& lexing->token[__strlen(lexing->token) - 1] != '"'
			&& __strlen(lexing->token) > 1)
		*/
		if ((lexing->type == WORD && before_type != HERE_DOC && before_type != REDIRECTION) && (lexing->token[0] != '"' || lexing->token[__strlen(lexing->token) - 1] != '"' || __strlen(lexing->token) > 1))
		{
			//fprintf(stderr, "go to split this token\n");
			if (!__split_token(lexing, msh))
				return (__putendl_fd("Malloc error", 2), 0);
		}
		before_type = lexing->type;
		lexing = lexing->next;
	}
	return (1);
}
