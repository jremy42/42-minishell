/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cree_tree_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 12:10:50 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 12:11:30 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*__reinit_node(t_node ***node_tab)
{
	free_split((char **)*node_tab);
	*node_tab = NULL;
	return (NULL);
}

t_node	*__give_node(int count, int reset)
{
	int				i;
	static t_node	**node_tab = NULL;
	static int		state = -1;

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

t_lexing	*__skip_parenthesis(t_lexing *lexing)
{
	int	parenthesis;

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
	t_lexing	*find;

	find = NULL;
	while (lexing)
	{
		if (lexing->type == P_LEFT)
			lexing = __skip_parenthesis(lexing);
		if (!lexing)
			break ;
		if (lexing->type == OPERATOR)
			find = lexing;
		lexing = lexing->next;
	}
	return (find);
}

int	trim_parenthesis(t_lexing **lexing, t_lexing **parenthesis)
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
