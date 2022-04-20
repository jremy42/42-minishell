#include "micro_ast.h"
#include "libft.h"

void print2DUtil(t_btree *root, int space)
{
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
    printf("%s\n", root->token);
 
    // Process left child
    print2DUtil(root->left, space);
}

void print2D(t_btree *root)
{
   // Pass initial space count as 0
   print2DUtil(root, 0);
}

void    __btree_destroy(t_btree *root)
{
    if (!root)
            return ;
    __btree_destroy(root->left);
    __btree_destroy(root->right);
    //free(root->token);
    free(root);
}

t_type __find_type(char *item)
{
    if(!item)
        return (-1); 
    if(__strncmp(item, "+", 2) == 0)
        return (PLUS);
    if(__strncmp(item, "-", 2) == 0)
        return (MINUS);
    if(__strncmp(item, "*", 2) == 0)
        return (MULTI);
    if(__strncmp(item, "/", 2) == 0)
        return (DIVIDE);
    else 
        return (NBR);
}

t_btree	*btree_create_node(char *token)
{
	t_btree	*new;

	new = malloc(sizeof(t_btree));
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->token = token;
    new->type = __find_type(token);
	return (new);
}

int __priority_cmp(char *find, char *tmp)
{
	return ((__find_type(find) & 1) - (__find_type(tmp) & 1));
}

int	 __skip_parenthesis(char **token, int i)
{
	int parenthesis;

	parenthesis = 1;
	i++;
	while (token[i] && parenthesis)
	{
		if (token[i][0] == '(')
			parenthesis++;
		if (token[i][0] == ')')
			parenthesis--;
		i++;
	}
	return (i);
}

char *__find_next_operator(char **token)
{
    char *find;
    char *tmp;
    int i;

    find = NULL;
    tmp = NULL;
    i = 0;
    while (token[i])
    {
		if (token[i][0] == '(')
		{
			i = __skip_parenthesis(token, i);
			continue ;
		}
        if (__find_type(token[i]) != NBR)
            tmp = token[i];
        if (find == NULL)
            find = tmp;
        else if (__priority_cmp(find, tmp) >= 0)
            find = tmp;
        i++;
    }
    return (find);
}
   
char **__split_token_left(char **token, char *next_op)
{
    char **left;
    int     size_left;

    size_left = 0;
	while(token[size_left] != next_op)
		size_left++;
    left = malloc(sizeof(char *) * (size_left + 1));
    if (!left)
        return(NULL);
    left[size_left] = NULL;
    while((--size_left )> -1 )
        left[size_left] = token[size_left];
    return (left);
}

char **__split_token_right(char **token, char *next_op)
{
    char **right;
    int     size_right;
    int     i;

    right = NULL;
    size_right = 0;
    i = 0;
    while(token[i] && token[i] != next_op)
        i++;
    i++;
    while (token[i + size_right])
        size_right++;
    right = malloc(sizeof(char *) * (size_right + 1));
    if (!right)
        return(NULL);
    right[size_right] = NULL;
    while(--size_right > -1)
        right[size_right] = token[size_right + i];
    return (right);
}

void print_token(char **token)
{
    int i;

    i = 0;
    while(token[i])
    {
        printf("%s", token[i]);
        i++;
    }
    printf("\n");
}

int	 __error_parenthesis(char **token)
{
    int i;
    int parenthesis;

    i = 0;
	parenthesis = 0;
	while (token[i])
	{
		if (token[i][0] == '(')
			parenthesis++;
		if (token[i][0] == ')')
			parenthesis++;
		i++;
	}
    return ((parenthesis%2));
}

int	trim_token(char ***token)
{
	int	i;

	i = 0;
	if ((*token)[i] && (*token)[i][0] == '(')
	{
		//free((*token)[i]);
		(*token)[i] = NULL;
		(*token)++;
	while ((*token)[i])
		i++;
    i--;
	//free((*token)[i]);
	(*token)[i] = NULL;
	return (1);
	}
	return (0);
}

int __create_tree(char **token, t_btree **root, int lvl)
{
    char	*next_op;
    char	**left_token;
    char	**right_token;
    t_btree	*new_node;
    int		res;
    int		res2;
	int		restrim;

    restrim = 0;
    if (!token)
    {
        *root = NULL;
        return (1);
    }
    if (__error_parenthesis(token))
    {
        return (printf("syntax Error\n"), 0);
    }
	restrim = trim_token(&token);
    next_op = __find_next_operator(token);
    if (!next_op)
    {
        new_node = btree_create_node(token[0]);
        if (!new_node)
            return (0);
        *root = new_node;
        free(token - restrim);
        return (1);
    }
    left_token = __split_token_left(token, next_op);
    right_token = __split_token_right(token, next_op);
    if (!left_token)
        return (0);
    if (!right_token)
        return (free(left_token), 0);
    new_node = btree_create_node(next_op); 
    if (!new_node)
        return (0);
    *root = new_node;
    res2 = __create_tree(right_token, &((*root)->right), lvl + 1);
    res = __create_tree(left_token, &((*root)->left), lvl + 1);
    if (!res || !res2)
    {
        __btree_destroy(*root);
        return (0);
    }
    if (lvl)
        free(token - restrim);
    return(1);
}

#include <string.h>

int	first_trim_token(char ***token)
{
	int	i;

	i = 0;
	if ((*token)[i] && (*token)[i][0] == '(')
	{
		free((*token)[i]);
		(*token)[i] = NULL;
		(*token)++;
	while ((*token)[i])
		i++;
    i--;
	free((*token)[i]);
	(*token)[i] = NULL;
	return (1);
	}
	return (0);
}

int main (int ac, char **av)
{
    t_btree *root;
    char **token;
    int j;
    (void)ac;
    int bracket;

    root = NULL;
    token = __split(av[1],' ');
    j = 0;
    bracket = first_trim_token(&token);
    while (token[j])
        j++;
    int res = 0;
    res = __create_tree(token, &root, 0);
    print2D(root);
	printf("resultat : %d\n", __execute_tree(root));
   __btree_destroy(root);
   int i = 0;
   while (i < j)
   {
       printf("i : %d\n", i);
       free(token[i]);
       i++;
   }
   free(token - bracket);
   return (res);
}  
//je suis un nbr et que right et left sont null je cree le node
// si je suis un nbr et que left 
// si je suis un nbr et que left est pris par un nbr ou un operateur je me met dans la branche droite
// si je suis un operateur et que left est nbr, je me creer et le nbr devient ma branche left
// si je suis un operateur et que left est un operateur, je devient le root.
