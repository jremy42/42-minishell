# include "minishell.h"
int __is_valide_operator(char *token)
{
	if (!__strncmp(token, "&&", 2))
		return (1);
	if (!__strncmp(token, "||", 2))
		return (1);
	if (!__strncmp(token, "|", 1))
		return (1);
	return (0);
}

void __synthax_error(char * str)
{
	write(2,"Syntax error near :", 19);
	write(2, str, __strlen(str));
	write(2, "\n", 1);
}

void __invalid_error(char * str)
{
	write(2,"Valid token but not handled in minishell:", 42);
	write(2, str, __strlen(str));
	write(2, "\n", 1);
}
/*
static int	trim_parenthesis_in_pipe(t_lexing **lexing, t_lexing **previous_lexing)
{
	t_lexing	*index;
	t_lexing	*save;

	if ((*lexing)->type == P_LEFT)
	{
		index = (*lexing)->next;
		while (index->next && (index->next->next || index->next->type != P_RIGHT))
		{
			if (index->next->type == OPERATOR || index->next->type == PIPE)
			{
				//return (printf("je retourne a cause de %s\n", index->next->token), 0);
				break ;
			}
			printf("token = %s\n", index->next->token);
			index = index->next;
		}
		if (index->next && index->type == P_RIGHT)
		{
			if (!index->next->next || index->next->next->type == OPERATOR || index->next->next->type == PIPE)
			{
				save = *lexing;
				*lexing = (*lexing)->next;
				(*previous_lexing)->next = *lexing;
				free(save->token);
				free(save);
				save = index->next;
				index->next = index->next->next;
				if (save)
					free(save->token);
				free(save);
				return(1);
			}
			return (0);
		}
		return (0);
	}
	else if ((*lexing)->type == P_RIGHT)
		return (0);
	else
		return (1);
}
*/

t_lexing *__synthax_checker(t_lexing *lexing, t_msh *msh)
{
	int parenthesis;

	parenthesis = 0;
	while (lexing)
	{
		if (lexing->type == P_LEFT)
			parenthesis++;
		if (lexing->type == P_RIGHT)
			parenthesis--;
		if (lexing->type == INVALID)
			return (__invalid_error(lexing->token), lexing);
		if (lexing->type == OPERATOR || lexing->type == PIPE || lexing->type == REDIRECTION)
		{

			// <=1 operateur || pipe
			if (!lexing->next || lexing->next->type == OPERATOR || lexing->next->type == PIPE || lexing->next->type == REDIRECTION)
			{
				msh->rv = 2;
				return (__synthax_error(lexing->token), lexing);
			}
		}
	if (parenthesis < 0)
	{
		msh->rv = 2;
		return(__synthax_error(lexing->token), lexing);
	}
		lexing = lexing->next;
	}
	if (parenthesis != 0)
	{
		msh->rv = 2;
		return (__synthax_error("end. Parenthesis no closed"), lexing);
	}
	return (NULL);
}
