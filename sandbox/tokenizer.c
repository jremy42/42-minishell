#include "libft.h"
#include <stdio.h>

typedef enum e_operator
{AND_IF, OR_IF, DLESS, DGREAT} t_operator;

typedef enum e_state
{UNQUOTE, S_QUOTE, D_QUOTE, OPERATOR} t_state;


int		is_operator_char(char c)
{
	if (__strchr("&|<>", c))
		return (1);
	return (0);
}

int		get_word(char **new_token, char *str, int i)
{
	t_state	state;
	char	*tmp;

	while (str[i] && str[i] != ' ' && !is_operator_char(str[i]))
	{
		state = (str[i] == 34) * S_QUOTE + (str[i] == 39) * D_QUOTE;
		if (state == UNQUOTE)
		{
			tmp = __strdup(" ");
			tmp[0] = str[i];
			*new_token = __strjoin(*new_token, tmp);
			printf("i : >%d<\n", i);
			printf("new_token en construction: >%s<\n", *new_token);
			free(tmp);
		}
		i++;
	}
	if (is_operator_char(str[i]))
		i--;
	if (!str[i])
		i--;
	return (i);
}

int		get_operator(char **new_token, char *str, int i)
{
	char	*tmp;

	while (str[i] && str[i] != ' ' && is_operator_char(str[i]))
	{
		tmp = __strdup(" ");
		tmp[0] = str[i];
		*new_token = __strjoin(*new_token, tmp);
		free(tmp);
		if (__strlen(*new_token) == 2)
			break ;
		i++;
	}
	if (!is_operator_char(str[i]))
		i--;
	if (!str[i])
		i--;
	return (i);
}

void	tokenize(char *s, t_list **start)
{
	//int		token_type;
	int		status_token;
	char	*new_token;
	int		i;
	t_list	*t_token;
	char	*str;

	//token_type = 0;
	status_token = 0;
	i = 0;
	new_token = NULL;
	str = __strtrim(s, " \f\t\r\v");
	while (str && str[i])
	{
		if (!status_token)
		{
			new_token = __strdup("");
			status_token = 1;
		}
		if (is_operator_char(str[i]))
			i = get_operator(&new_token, str, i);
		else
			i = get_word(&new_token, str, i);
		t_token = __lstnew(new_token);
		if (!t_token)
			return ;
		printf("new_token : >%s<\n", new_token);
		__lstadd_back(start, t_token);
		status_token = 0;
		i++;
		while (str[i] == ' ')
			i++;
	}
	if (!__strlen(new_token))
	{
			new_token = __strdup("EOI");
		t_token = __lstnew(new_token);
		if (!t_token)
			return ;
		printf("new_token : >%s<\n", new_token);
		__lstadd_back(start, t_token);
	}
	free(str);
	return ;
}

int main(int ac, char *av[])
{
	(void)ac;
	t_list	*start;
	t_list	*index;

	start = NULL;
	if (ac != 2)
		return 42;
	tokenize(av[1], &start);

	index = start;
	while (index)
	{
		printf("Le token vaut : >%s<\n", (char *)index->content); 
		index = index->next;
	}
	__lstclear(&start, &free);
	return (0);
}
