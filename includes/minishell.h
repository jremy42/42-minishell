#include <errno.h>
#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
#include <signal.h>

# define PATH_MAX 4096
# define PROMPT_MAX 5000
# define __SUCCESS 0
# define __FAIL 1
# define __MALLOC 240
# define RESET   "\033[0m"
# define BLACK   "\033[30m"      /* Black */
# define RED     "\033[31m"      /* Red */
# define GREEN   "\033[32m"      /* Green */
# define YELLOW  "\033[33m"      /* Yellow */
# define BLUE    "\033[34m"      /* Blue */
# define MAGENTA "\033[35m"      /* Magenta */
# define CYAN    "\033[36m"      /* Cyan */
# define WHITE   "\033[37m"      /* White */
# define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
# define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
# define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
# define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
# define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
# define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
# define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
# define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

# define COUNT 10

# ifndef DEBUG
# define DEBUG 0
# endif


typedef enum	e_state
{UNQUOTE, S_QUOTE, D_QUOTE, BACKSLASH} t_state;

typedef enum	e_token_type
{OPERATOR, PIPE, NEW_LINE, WORD, REDIRECTION, HERE_DOC, P_LEFT, P_RIGHT, INVALID} t_token_type;

typedef enum e_kind_node
{OR = 1, AND, SEQUENCE} t_kind_node;

typedef enum e_redir_type
{LESS, GREAT, DGREAT, H_D} t_redir_type;

typedef enum	e_globe_type
{FIRST, MIDDLE, LAST} t_globe_type;

typedef struct s_redirect
{
	t_redir_type type;
	char *file_name;
	struct s_redirect *next;
} t_redirect;
 
//Faire les redirections juste avant de creer la liste finale des args (nom du prog et les vrai parametres)
// Le *msh permettra de modifier la RV et de recuperer l'env necessaire a l'exe

typedef struct s_cmd
{
	int		redirection[2];
	int		index;
	char	**arg;
	struct s_msh *msh;
	struct s_cmd *next;
	t_redirect *redirect;
}	t_cmd;


typedef struct s_lexing
{
	char *token;
	t_token_type type;
	struct s_lexing *next;
} t_lexing;

typedef struct s_glob
{
	char *to_find;
	t_globe_type type;
	struct s_glob *next;
} t_glob;


// A la construction de l'AST, le node obtient son kind, son node right et left.
// La donnee est le pointeur sur un t_lexing (cad une liste chainee de token)
// Dans les passage successifs suivants, on va:
//1 : Faire l'expand et MAJ du t_lexing
//2 : Faire les redirections et Transfo du t_lexing* en t_cmd * (le nombre de parametres est maintenant connu)
//3 : Execution des noeud et parcours de l'arbre en fonction des valeurs de retour
typedef struct s_node
{
	t_kind_node		kind;
	//t_cmd			*cmd;
	struct s_node	*left;
	struct s_node	*right;
	t_lexing		*leaf_lexing;
} t_node;

typedef struct s_msh
{
	int		rv;
	t_node	*root;
	char	*prompt;
	char	***envp;
	char	*error_string;
	char 	*error_value;
}	t_msh;
/*
decouper en token / mots / operateur
cmd simple 
exe
fonction de gestion des fd
*/
//built-in

int	__pwd(int fd);
int	__echo(char **str, int fd);
int	__cd(char *new_path, t_msh *msh);
int	__env(t_msh *msh);
int	__unset(char **arg, t_msh *msh);
int	__export(char **key_val, t_msh *msh);

//Export utils

char	*get_key(t_msh *msh, char *key);
int		modify_status_key_val(t_msh *msh, char *key_val);
int		add_key_val(t_msh *msh, char *key_val, int i);
int		update_key_val(t_msh *msh, char *key_val);
int		key_exist(t_msh *msh, char *key);
int		get_envp_size(t_msh *msh);
int	join_key_val(t_msh *msh, char *key_val);

//cd utils

char	*create_absolut_pwd(char *pwd, char *new_path);
int	update_oldpwd(t_msh *msh);
int	update_pwd(t_msh *msh);
int	chdir_absolute_path(char *new_path);
int	chdir_previous(t_msh *msh);




//Others
void    __signal(int signal);
void    __signal_treat(int signal);
int __exit(t_msh *msh);

//tokenizer

t_state	__return_state(char c, int state, int slash_state);
int		__add_char_to_token(char c, char **token);
int		__need_to_escape(int i, t_state state, char *str);
int		__is_operator_char(char c);

int		__tokenize(char *s, t_list **start);
int		__get_operator(char **new_token, char *str, int i);
int		__get_word(char **new_token, char *str, int i);
int		__adjust_i(char *str, int i, int state);
int		__add_token(char *str, t_list **start);
int		__skip_spaces(char *str, int *i);
int		__create_new_token(char **str);
int		__treat_newline(t_list **start, int *i);
int		__init_token_if_none(char **str, int *token_status);

// lexing
int	__lexing(t_list *token, t_lexing **lexing);
t_lexing *__synthax_checker(t_lexing *lexing);
int	__print_lexing(t_lexing *lexing);
void	__lexing_full_list_clear(t_lexing *start);
void	__lexing_node_list_clear(t_lexing *start);

// Gardening

int __create_tree(t_lexing *lexing, t_node **root);
int __execute_tree(t_node *current_node, t_msh *msh);
void __destroy_tree(t_node **current_node);


// parsing exe 
t_cmd *miniparsing(t_lexing *lexing);
int print_cmd(t_cmd *cmd);
int execute_seq(t_cmd *cmd, t_msh *msh);
int add_next_cmd(t_cmd **start, t_lexing **lexing, t_msh *msh, int index);
t_cmd *create_cmd_list(t_lexing *lexing, t_msh *msh);
int print_cmd_lst(t_cmd *cmd);
void	__cmd_node_list_clear(t_cmd *start);
int __clean_tmp_hd(t_cmd *cmd);

//heredoc

int __handle_here_doc(t_lexing *lexing, t_lexing *end, t_msh *msh);

//expande
int __parameter_expand_token(t_lexing *lexing, t_msh *msh);
int __handle_wildcards(t_msh *msh, t_lexing *lexing);
t_lexing	*__lexnew(char *content);
int __insert_token(t_lexing *lexing, char *new_glob_match, int reset, t_lexing *true_end);

#endif
