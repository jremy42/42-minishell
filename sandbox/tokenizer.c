#include "libft.h"
#include <stdio.h>

enum e_operator {AND_IF, OR_IF, DLESS, DGREAT};


int		is_operator(char *token)
{
	if (!token)
		return (-1);
	if (__strlen(token) > 2 || __strlen(token) == 0)
		return (0);
	if (__strlen(token) > 0 && __strchr("&|<>", token[0]))
	{
		if (__strlen(token) > 1 && token[0] == token[1])
			return (1);
	}
	return (0);
}
	

void	tokenize(char *str, t_list **start)
{
	int		status_token;
	int		token_type;
	char	*new_token;
	char	*tmp;
	int		i;
	t_list	*t_token;

	status_token = 0;
	token_type = 0;
	i = 0;
	while (str[i])
	{
		if (!status_token)
		{
			new_token = __strdup(" ");
			new_token[0] = str[i];
			status_token = 1;
		}
		else
		{
			tmp = __strdup(" ");
			tmp[0] = str[i];
			new_token = __strjoin(new_token, tmp);
			free(tmp);
		}
		i++;
	}
	if (!__strlen(new_token))
		new_token = __strdup("EOI");
	t_token = __lstnew(new_token);
	if (!t_token)
		return ;
	__lstadd_back(start, t_token);
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
		printf("Le token vaut : %s\n", (char *)start->content); 
		index = index->next;
	}
	__lstclear(&start, &free);
	return (0);
}
