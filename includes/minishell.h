/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/31 12:29:58 by jremy             #+#    #+#             */
/*   Updated: 2022/03/31 15:01:18 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <errno.h>
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
# include <signal.h>

//# define PATH_MAX 4096
# define PROMPT_MAX 5000
# define __SUCCESS 0
# define __FAIL 1
# define __MALLOC 240
# define __SYNTAX_ERROR -2
# define __MALLOC_TOKEN -1
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
#  define DEBUG 0
# endif

typedef enum e_state {
	UNQUOTE,
	S_QUOTE,
	D_QUOTE,
	BACKSLASH
}	t_state;

typedef enum e_token_type{
	OPERATOR,
	PIPE,
	NEW_LINE,
	WORD,
	REDIRECTION,
	HERE_DOC,
	P_LEFT,
	P_RIGHT,
	INVALID
}	t_token_type;

typedef enum e_kind_node {
	OR = 1,
	AND,
	SEQUENCE
}	t_kind_node;

typedef enum e_redir_type {
	LESS,
	GREAT,
	DGREAT,
	H_D
}	t_redir_type;

typedef enum e_globe_type {
	FIRST,
	MIDDLE,
	LAST
}	t_globe_type;

typedef struct s_redirect
{
	t_redir_type		type;
	char				*file_name;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_cmd
{
	int				redirection[2];
	int				index;
	char			**arg;
	struct s_msh	*msh;
	struct s_cmd	*next;
	t_redirect		*redirect;
}	t_cmd;

typedef struct s_lexing
{
	char			*token;
	t_token_type	type;
	struct s_lexing	*next;
}	t_lexing;

typedef struct s_glob
{
	char			*to_find;
	t_globe_type	type;
	struct s_glob	*next;
}	t_glob;

typedef struct s_user_input
{
	t_list		*token;
	t_lexing	*lexing;
	char		*to_tokenize;
	t_lexing	*first_error;
	int			syntax_tree;
	t_lexing	*parenthesis;
	int			ret_hd;
	int			ret_tokenize;
}	t_user_input;

typedef struct s_node
{
	t_kind_node		kind;
	struct s_node	*left;
	struct s_node	*right;
	t_lexing		*leaf_lexing;
}	t_node;

typedef struct s_msh
{
	int		rv;
	int		syntax_error;
	t_node	*root;
	char	*prompt;
	char	***envp;
	char	*error_string;
	char	*error_value;
	char	**all_input;
}	t_msh;

//built-in

int				__pwd(int fd);
int				__echo(char **str, int fd);
int				__cd(char *new_path, t_msh *msh);
int				__env(t_msh *msh);
int				__unset(char **arg, t_msh *msh);
int				__export(char **key_val, t_msh *msh);
int				__bin_exit(char **key_val, t_msh *msh, t_cmd *cmd);
//Export utils
char			*get_key(t_msh *msh, char *key);
int				modify_status_key_val(t_msh *msh, char *key_val);
int				add_key_val(t_msh *msh, char *key_val, int i);
int				update_key_val(t_msh *msh, char *key_val);
int				key_exist(t_msh *msh, char *key);
int				get_envp_size(t_msh *msh);
int				join_key_val(t_msh *msh, char *key_val);
int				get_number_exported_values(char ***envp);
int				print_exported_values(char ***envp);
int				__check_valid_identifier(char *key_val);

//cd utils

char			*create_absolut_pwd(char *pwd, char *new_path);
int				update_oldpwd(t_msh *msh);
int				update_pwd(t_msh *msh);
int				chdir_absolute_path(char *new_path, t_msh *msh);
int				chdir_previous(t_msh *msh);
int				__access_dir(char *dir, char *true_dir);

//Others
void			__signal(int signal);
void			__signal_treat(int signal);
void			__destroy_tree(t_node **current_node);
void			destroy_env(t_msh *msh);
int				__exit(t_msh *msh);
int				__exit_error(t_msh *msh, int error, char *str);
int				update_shlvl(t_msh *msh);
int				__get_pos_last_dir(char *cwd);
int				__check_input(char *arg, char **to_tokenize, t_msh *msh);
void			__update_rv(t_msh *msh);
void			__clean_inputs(char **inputs, t_msh *msh, char *arg);
t_user_input	*__init_user_input_struct(t_user_input *ui);
int				get_env(t_msh *msh, char *envp[], int size);
int				get_size_env(char *envp[]);
void			destroy_env(t_msh *msh);
void			__destroy_tree(t_node **current_node);
int				print_error(char *cmd, char *arg, char *error_msg);

//tokenizer

t_state			__return_state(char c, int state, int slash_state);
int				__add_char_to_token(char c, char **token);
int				__need_to_escape(int i, t_state state, char *str);
int				__is_operator_char(char c);

int				__tokenize(char *s, t_list **start, t_msh *msh);
int				__get_operator(char **new_token, char *str, int i);
int				__get_word(char **new_token, char *str, int i, t_msh *msh);
int				__adjust_i(char *str, int i, int state, t_msh *msh);
int				__add_token(char *str, t_list **start);
int				__skip_spaces(char *str, int *i);
int				__create_new_token(char **str);
int				__treat_newline(t_list **start, t_msh *msh);
int				__init_token_if_none(char **str, int *token_status);

//syntax
void			__invalid_error(char *str);
void			__synthax_error(char *str);
int				__is_valide_operator(char *token);

// lexing
int				__lexing(t_list *token, t_lexing **lexing);
t_lexing		*__synthax_checker(t_lexing *lexing, t_msh *msh);
int				__print_lexing(t_lexing *lexing);
void			__lexing_full_list_clear(t_lexing **start);
void			__lexing_node_list_clear(t_lexing *start);
void			__lexing_not_in_tree_list_clear(t_lexing *start);
void			__lexadd_back(t_lexing **alst, t_lexing *new);
int				__count_node(t_lexing *lexing);
int				__is_operator(char *content);
// Gardening

int				__create_tree(t_lexing *lexing, t_node **root,
					t_lexing **parenthesis);
int				__execute_tree(t_node *current_node, t_msh *msh);
void			__destroy_tree(t_node **current_node);
t_node			*__give_node(int count, int reset);
int				trim_parenthesis(t_lexing **lexing, t_lexing **parenthesis);
t_lexing		*__find_next_operator(t_lexing *lexing);
t_lexing		*__skip_parenthesis(t_lexing *lexing);
t_node			*__give_node(int count, int reset);
t_node			*__reinit_node(t_node ***node_tab);

// parsing exe 
t_cmd			*miniparsing(t_lexing *lexing);
int				execute_seq(t_cmd *cmd, t_msh *msh);
int				add_next_cmd(t_cmd **start, t_lexing **lexing,
					t_msh *msh, int index);
t_cmd			*create_cmd_list(t_lexing *lexing, t_msh *msh);
void			__cmd_node_list_clear(t_cmd *start);
void			__cmd_full_list_clear(t_cmd *start);
int				__clean_tmp_hd(t_cmd *cmd);
int				__trim_quote(char **eof, int *quote);
char			*__get_stdin(char *eof, t_msh *msh);
void			__treat_eof(char *line, char *eof, t_msh *msh);
//heredoc
int				__handle_here_doc(t_lexing *lexing, t_lexing *end, t_msh *msh);
void			__signal_hd(int signal);
int				here_doc_handler(t_user_input *ui, t_msh *msh);
void			__init_child_hd(char *eof, t_lexing *lex,
					t_msh *msh, t_lexing *sv);
int				__get_user_input(char **eof, t_msh *msh);
//expande
int				__parameter_expand_token(t_lexing *lexing, t_msh *msh);
int				__handle_wildcards(t_lexing *lexing);
t_lexing		*__lexnew(char *content);
int				__insert_token(t_lexing *lexing, char *new_glob_match,
					int reset, t_lexing *true_end);
int				__field_spliting_token(t_lexing *lexing, t_msh *msh);
int				__quote_removal_token(t_lexing *lexing);
int				__quote_removal_glob(t_glob *glob);
int				__move_to_next_unquoted_star(char *str);
int				__move_to_next_unquoted_char(char *str, char c);
int				__move_to_next_unquoted_charset(char *str, char *charset);
int				__get_char_quote_status(char *str, char *to_find);
size_t			__is_in_charset(char c, char *charset);
int				__key_match_canditate(char *cndte, char *env_key,
					t_msh *msh, int j);
int				__sub_cdnte(char *env_key, char *candidate,
					char **expanded_token, char *env_value);
char			*__get_candidate(char *start_word, int *i);
int				__treat_last_rv(char **expanded_token, int *i, t_msh *msh);
char			*__get_key_from_key_val(char *str);
char			**__split_unquoted_charset(char *s, char *charset);
// globe
t_glob			*__glob_new(char *content, t_globe_type type, int size);
int				__move_to_next_unquoted_charset(char *str, char *charset);
int				__move_to_next_unquoted_char(char *str, char c);
void			__globadd_back(t_glob **alst, t_glob *new);
void			__glob_list_clear(t_glob *start);
t_glob			*__glob_new(char *content, t_globe_type type, int size);
int				__init_dir_content(t_list **dir_content);
int				__get_char_quote_status(char *str, char *to_find);
t_globe_type	__get_next_state(t_globe_type current_state, char *token);
int				__add_new_glob(char *to_glob_expand,
					t_globe_type *state, t_glob **glob, int j);
void			__get_glob_size(int glob_len, int *i, int *j, char **tge);
int				__add_remaining_globs(char **tge, t_glob **glob,
					t_globe_type state);
t_glob			*__create_glob_lst(char **to_glob_expand);
int				__find_end(char *file_name, t_glob *glob_lst);
#endif
