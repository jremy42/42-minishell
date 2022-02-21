#include "minishell.h"


t_lexing *__skip_parenthesis(t_lexing *lexing)
{
	int parenthesis;

	parenthesis = 1;

	lexing = lexing->next;
	while (lexing && parenthesis)
	{
		if (lexing->type == P_LEFT)
			parenthesis++;
		if (lexing->type == P_RIGHT)
			parenthesis--;
		lexing = lexing->next;
	}
	printf("%s\n", lexing->token);
	return (lexing);
}

t_lexing	*__find_next_operator(t_lexing *lexing)
{
	t_lexing *find;

	find = NULL;
    while (lexing)
    {
		if (lexing->type == P_LEFT)
			lexing = __skip_parenthesis(lexing);

        if (lexing->type == OPERATOR)
            find = lexing;
		lexing = lexing->next;
    }
    return (find);
}

static int	trim_parenthesis(t_lexing **lexing)
{
	t_lexing	*index;

	if ((*lexing)->type == P_LEFT)
	{
		index = (*lexing)->next;
		while (index->next->next)
			index = index->next;
		if (index->next->type == P_RIGHT)
		{
			*lexing = (*lexing)->next;
			index->next = NULL;	
		}
		return (1);
	}
	else if ((*lexing)->type == P_RIGHT)
		return (0);
	else
		return (1);
}

t_node	*btree_create_node_sequence(t_lexing *lexing)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->kind = SEQUENCE;
    //new->cmd = create_cmd_array(lexing);
	new->cmd = NULL;
    new->tmp = lexing;
	return (new);
}

t_node	*btree_create_node_operator(t_lexing *lexing)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->kind = !__strcmp(lexing->token, "||") * OR
		+ !__strcmp(lexing->token, "&&") * AND;
    new->cmd = NULL;
	return (new);
}

t_lexing	*split_lexing_left(t_lexing *lexing, t_lexing *next_operator)
{
	t_lexing	*save;
	
	save = lexing;
	while (lexing->next != next_operator)
		lexing = lexing->next;
	lexing->next = NULL;
	return (save);
}

t_lexing	*split_lexing_right(t_lexing *lexing, t_lexing *next_operator)
{
	while (lexing != next_operator)
		lexing = lexing->next;
	lexing = lexing->next;
	return (lexing);
}

int __create_tree(t_lexing *lexing, t_node **root)
{
	t_lexing	*next_operator;

	
	printf("\n\n\n");
	//__print_lexing(lexing);

	next_operator = __find_next_operator(lexing);
	if (!next_operator)
	{
		if (!trim_parenthesis(&lexing))
			return (write(2, "Error\n", 6), 0);
		next_operator = __find_next_operator(lexing);
	}
	if (!next_operator)
	{
		*root = btree_create_node_sequence(lexing);
		return (1);
	}
	printf("next operator : [%s]\n", next_operator->token);
	*root = btree_create_node_operator(next_operator);
	if (!*root)
		return (0);
	__create_tree(split_lexing_right(lexing, next_operator),&((*root)->right));
	__create_tree(split_lexing_left(lexing, next_operator), &((*root)->left));
	return (1);
}
