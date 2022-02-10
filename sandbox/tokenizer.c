#include "libft.h"
#include <stdio.h>


typedef enum e_operator
{AND_IF, OR_IF, DLESS, DGREAT} t_operator;

typedef enum e_state
{UNQUOTE, S_QUOTE, D_QUOTE, BACKSLASH} t_state;


int		is_operator_char(char c)
{
	if (__strchr("&|<>", c))
		return (1);
	return (0);
}

int		need_to_escape(int i, t_state state, char *str)
{
	if (state == S_QUOTE)
		return (0);
	if (state == D_QUOTE && __strchr("$`\\\"", str[i + 1]))
		return (1);
	if (state == UNQUOTE)
		return (1);
	return (1);
}

int		get_word(char **new_token, char *str, int i)
{
	t_state	state;
	t_state	slash_state;
	char	*tmp;

	state = UNQUOTE;
	slash_state = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] && need_to_escape(i, state, str))
		{
			slash_state = BACKSLASH;
			i++;
			continue ;
		}
		if (str[i] == 34 && state == D_QUOTE && slash_state)
			state = UNQUOTE;
		else if (str[i] == 39 && state == S_QUOTE && slash_state)
			state = UNQUOTE;
		else if (state == UNQUOTE)
			state = (str[i] == 39) * S_QUOTE + (str[i] == 34) * D_QUOTE;
		printf("i : %c state = %d, bs_state = %d\n",str[i], state, slash_state);
		if (state == UNQUOTE && slash_state == 0)
		{
			if (is_operator_char(str[i]) || __strchr("\f\t\n\r\v", str[i]))
				break;
		}
			tmp = __strdup(" ");
			tmp[0] = str[i];
			*new_token = __strjoin(*new_token, tmp);
			printf("new_token en construction: >%s<\n", *new_token);
			free(tmp);
		i++;
		slash_state = 0;

	}
	if (state != UNQUOTE)
		return (24 - write(2, "Error : Quote not closed\n", 25));
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

int	tokenize(char *s, t_list **start)
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
		if (str[i] == '\n')
		{
			t_token = __lstnew(__strdup("<newline>"));
			if (!t_token)
				return (0);
			__lstadd_back(start, t_token);
			i++;
			continue ;
		}
		if (!status_token)
		{
			new_token = __strdup("");
			status_token = 1;
		}
		if (is_operator_char(str[i]))
			i = get_operator(&new_token, str, i);
		else
			i = get_word(&new_token, str, i);
		if (i < 0)
			return (-1);
		t_token = __lstnew(new_token);
		if (!t_token)
			return (-2);
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
			return (-2);
		printf("new_token : >%s<\n", new_token);
		__lstadd_back(start, t_token);
	}
	free(str);
	return (0);
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
