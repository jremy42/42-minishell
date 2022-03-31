/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 10:17:40 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/31 15:57:20 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*btree_create_node_sequence(t_lexing *lexing)
{
	t_node	*new;

	new = __give_node(0, 0, 0);
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

	new = __give_node(0, 0, 0);
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

int	__create_tree(t_lexing *lexing, t_node **root, t_lexing **parenthesis)
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
	if (!__create_tree(split_lexing_right(lexing, next_operator),
			&((*root)->right), parenthesis))
		return (0);
	if (!__create_tree(split_lexing_left(lexing, next_operator),
			&((*root)->left), parenthesis))
		return (0);
	return (1);
}
