/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_tree_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fle-blay <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 17:59:20 by fle-blay          #+#    #+#             */
/*   Updated: 2022/03/31 17:59:53 by fle-blay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*__reinit_node(t_node ***node_tab)
{
	free_split((char **)*node_tab);
	*node_tab = NULL;
	return (NULL);
}
