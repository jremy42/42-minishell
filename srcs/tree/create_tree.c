#include "minishell.h"


t_node *__reinit_node(t_node ***node_tab)
{
	free_split((char**)*node_tab);
	*node_tab = NULL;
	return (NULL);
}

t_node *__give_node(int count, int reset)
{
	int i;
	static int state = -1;
	static t_node	**node_tab = NULL;

	i = -1;
	if (reset)
	{
		state = 0;
		free(node_tab);
		if (count == 0)
			return (NULL);
		node_tab = (t_node **)malloc(count * sizeof(t_node *));
		if (!node_tab)
			return (NULL);
		while (++i < count)
		{
			node_tab[i] = (t_node *)malloc(sizeof(t_node));
			if (!node_tab[i])
				return (__reinit_node(&node_tab));
		}
		return (node_tab[0]);
	}
	else
		return (node_tab[state++]);
}

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
		if (!lexing)
			break;
		if (lexing->type == OPERATOR)
			find = lexing;
		lexing = lexing->next;
	}
	return (find);
}

static int	trim_parenthesis(t_lexing **lexing, t_lexing **parenthesis)
{
	t_lexing	*index;
	t_lexing	*rebirth_2_burne;
	t_lexing	*rebirth_2_burne2;

	if ((*lexing)->type == P_LEFT)
	{
		index = (*lexing)->next;
		rebirth_2_burne = *lexing;
		while (index->next->next)
			index = index->next;
		if (index->next->type == P_RIGHT)
		{
			*lexing = (*lexing)->next;
			rebirth_2_burne2 = index->next;
			index->next = NULL;	
			rebirth_2_burne->next = NULL;
			rebirth_2_burne2->next = NULL;
			__lexadd_back(parenthesis, rebirth_2_burne);
			__lexadd_back(parenthesis, rebirth_2_burne2);
		}
		return (1);
	}
	else if ((*lexing)->type == P_RIGHT)
		return (0);
	return (1);
}

t_node	*btree_create_node_sequence(t_lexing *lexing)
{
	t_node	*new;

	new = __give_node(0, 0);
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->kind = SEQUENCE;
	new->leaf_lexing = lexing;
	return (new);
}



t_node	*btree_create_node_operator(t_lexing *lexing)
{
	t_node	*new;

	new = __give_node(0, 0);
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->kind = !__strcmp(lexing->token, "||") * OR
		+ !__strcmp(lexing->token, "&&") * AND;
	new->leaf_lexing = lexing;
	free(lexing->token);
	lexing->token = NULL;	
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

int __create_tree(t_lexing *lexing, t_node **root, t_lexing **parenthesis)
{
	t_lexing	*next_operator;

	next_operator = __find_next_operator(lexing);
	if (!next_operator)
	{
		if (!trim_parenthesis(&lexing, parenthesis))
			return (write(2, "Syntax Error\n", 6), 0);
		next_operator = __find_next_operator(lexing);
	}
	if (!next_operator)
	{
		*root = btree_create_node_sequence(lexing);
		return (1);
	}
	*root = btree_create_node_operator(next_operator);
	if(!__create_tree(split_lexing_right(lexing, next_operator),&((*root)->right), parenthesis))
		return (0);
	if(!__create_tree(split_lexing_left(lexing, next_operator), &((*root)->left), parenthesis))
		return (0);
	return (1);
}