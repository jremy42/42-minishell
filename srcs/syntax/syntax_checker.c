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


t_lexing *__synthax_checker(t_lexing *lexing)
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
		if (lexing->type == OPERATOR || lexing->type == PIPE)
		{
			// <=1 operateur || pipe
			if (!lexing->next || lexing->next->type == OPERATOR || lexing->next->type == PIPE)
				return (__synthax_error(lexing->token), lexing);
		}
	if (parenthesis < 0)
			return(__synthax_error(lexing->token), lexing);
		lexing = lexing->next;
	}
	if (parenthesis)
		return (__synthax_error("end. Parenthesis no closed"), lexing);
	return (NULL);
}
