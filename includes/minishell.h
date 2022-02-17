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

typedef enum	e_operator
{AND_IF, OR_IF, DLESS, DGREAT} t_operator;

typedef enum	e_state
{UNQUOTE, S_QUOTE, D_QUOTE, BACKSLASH} t_state;

typedef enum	e_ltype
{OPERATOR, WORD, NEW_LINE} t_ltype;

typedef struct s_cmd
{
	char			*cmd;
	char			*flag;
	int				index;
	int				bin;
	int				fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_msh
{
	int		rv;
	t_cmd	*cmd;
	char	*prompt;
	char	***envp;
}	t_msh;

typedef struct s_gw
{
	t_state sl_st;
	t_state st;
	char	*tmp;
} t_gw;

typedef struct s_token
{
	int		status;
	int		i;
	char	*new_token;
} t_token;

typedef struct s_lexing
{
	char *token;
	t_ltype type;
	struct s_lexing *next;
} t_lexing;

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
char	*__add_char_nt(char *tmp, char c, char **new_token);
int		__need_to_escape(int i, t_state state, char *str);
int		__is_operator_char(char c);

int		__tokenize(char *s, t_list **start);
int		__get_operator(char **new_token, char *str, int i);
int		__get_word(char **new_token, char *str, int i);
int		__adjust_i(char *str, int i, int state);
int		__add_token(char *str, t_list **start);
int		__adjust_space(char *str, int i);
int		__create_new_token(char **str);


#endif
