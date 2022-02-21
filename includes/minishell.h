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

typedef enum	e_state
{UNQUOTE, S_QUOTE, D_QUOTE, BACKSLASH} t_state;

typedef enum	e_token_type
{OPERATOR, PIPE, NEW_LINE, WORD, REDIRECTION, P_LEFT, P_RIGHT, INVALID} t_token_type;

typedef enum e_kind_node
{OR = 1, AND, SEQUENCE} t_kind_node;

typedef struct s_cmd
{
	int		redirection[2];
	char	*arg[];
}	t_cmd;


typedef struct s_lexing
{
	char *token;
	t_token_type type;
	struct s_lexing *next;
} t_lexing;

typedef struct s_node
{
	t_kind_node		kind;
	t_cmd			*cmd;
	struct s_node	*left;
	struct s_node	*right;
	t_lexing		*tmp;
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

//Others
void    __signal(int signal);
int __exit(t_msh *msh);

//tokenizer

int		__return_state(char c, int state, int slash_state);
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
int	__synthax_checker(t_lexing *lexing);
void	__print_lexing(t_lexing *lexing);

// Gardening

int __create_tree(t_lexing *lexing, t_node **root);

#endif
