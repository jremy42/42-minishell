/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/22 10:08:17 by jremy             #+#    #+#             */
/*   Updated: 2022/03/28 18:54:03 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int __print_lexing(t_lexing *lexing)
{
	while (lexing)
	{
		fprintf(stderr, "Le token vaut : >%s< et de type = %d\n", lexing->token, lexing->type); 
		lexing = lexing->next;
	}	
	return (1);
}

void print2DUtil(t_node *root, int space)
{
	t_lexing *index;
	
	// Base case
	if (root == NULL)
		return;
 
	// Increase distance between levels
	space += COUNT;
 
	// Process right child first
	print2DUtil(root->right, space);
 
	// Print current node after space
	// count
	printf("\n");
	for (int i = COUNT; i < space; i++)
		printf(" ");
	printf("%d\n", root->kind);
	if (root->kind == SEQUENCE)
	{
		index = root->leaf_lexing;
		while (index)
		{
			printf("[%s] ", index->token);
			index = index->next;
		}
	}
 
	// Process left child
	print2DUtil(root->left, space);
}

int print2D(t_node *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
   printf("\n");
   return (1);
}