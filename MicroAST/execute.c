#include "micro_ast.h"
#include "libft.h"

int	__execute_tree(t_btree	*root)
{
	if (!root)
		return (0);
	if (root->type == PLUS)
		return (__execute_tree(root->left) + __execute_tree(root->right));
	if (root->type == MINUS)
		return (__execute_tree(root->left) - __execute_tree(root->right));
	if (root->type == MULTI)
		return (__execute_tree(root->left) * __execute_tree(root->right));
	if (root->type == DIVIDE)
		return (__execute_tree(root->left) / __execute_tree(root->right));
	else
		return (__atoi(root->token));
}
