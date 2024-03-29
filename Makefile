SRC = srcs/minishell.c \
srcs/bin/pwd/pwd.c\
srcs/bin/echo/echo.c\
srcs/bin/cd/cd_utils.c\
srcs/bin/cd/cd_utils2.c\
srcs/bin/cd/cd.c\
srcs/bin/env/env.c\
srcs/bin/export/export.c\
srcs/bin/export/export_utils.c\
srcs/bin/export/export_utils_2.c\
srcs/bin/export/export_utils_3.c\
srcs/bin/export/export_printer.c\
srcs/bin/unset/unset.c\
srcs/signal.c\
srcs/exit.c\
srcs/tokenizer/tokenizer_utils.c\
srcs/tokenizer/tokenizer_utils2.c\
srcs/lexer/lexer.c\
srcs/tokenizer/tokenizer.c\
srcs/syntax/syntax_checker.c\
srcs/tree/create_tree.c\
srcs/tree/exec_tree.c\
srcs/exe/exe_childs.c\
srcs/exe/init_seq.c\
srcs/exe/create_cmd.c\
srcs/exe/create_redirect.c\
srcs/exe/launcher_forks.c\
srcs/exe/redirect.c\
srcs/exe/redirect_utils.c\
srcs/here_doc/here_doc.c\
srcs/expand/expand.c\
srcs/expand/globe.c\
srcs/expand/globe_utils_1.c\
srcs/expand/globe_utils_2.c\
srcs/expand/globe_utils_3.c\
srcs/exe/exe.c\
srcs/bin/exit/exit.c\
srcs/expand/field_spliting.c\
srcs/expand/quote_remove.c\
srcs/split_quote.c\
srcs/exe/create_cmd_utils.c\
srcs/exe/exe_childs_utils.c\
srcs/exe/exe_utils.c\
srcs/exe/launcher_forks_utils.c\
srcs/env.c\
srcs/minishell_utils.c\
srcs/expand/expand_utils.c\
srcs/expand/expand_utils_2.c\
srcs/here_doc/here_doc_utils.c\
srcs/here_doc/here_doc_utils2.c\
srcs/lexer/lexer_utils.c\
srcs/syntax/syntax_utils.c\
srcs/exit_utils.c\
srcs/tree/create_tree_utils.c\
srcs/tree/create_tree_utils_2.c\
srcs/tree/exec_tree_utils.c\
srcs/expand/clean_token.c\
srcs/print_prompt.c\
srcs/expand/expand_utils_3.c

IFLAGS =  -I includes/ -I libft/includes
CFLAGS = -Wall -Wextra -Werror -g3
#CFLAGS += -fsanitize=address
CC = cc
OBJ = $(addprefix $(OBJS_PATH), $(SRC:.c=.o))
HEADER = includes/minishell.h
LIBFT = ./libft
OBJS_PATH = ./objs/
NAME = minishell

_GREY=	$'\033[30m
_RED=	$'\033[31m
_GREEN=	$'\033[32m
_YELLOW=$'\033[33m
_BLUE=	$'\033[34m

_PURPLE=$'\033[35m
_CYAN=	$'\033[36m
_WHITE=	$'\033[37m
_END= $'\033[0m

all: $(NAME)

ifeq ($(shell uname -s), Darwin)

MAC_DIR_READLINE=$(shell brew --prefix readline)

$(OBJS_PATH)%.o: %.c $(HEADER)
		mkdir -p $(dir $@)
		@$(CC) $(CFLAGS) $(IFLAGS) -c $< -I $(MAC_DIR_READLINE)/include -o $@
		@printf "%-15s ${_YELLOW}${_BOLD}$<${_END}...\n" "Compiling"

$(NAME): $(OBJ) ${HEADER} ${LIBFT}
		@printf "%-15s ${_CYAN}${_BOLD}libft${_END}...\n" "Compiling"
		@make -C ${LIBFT} > /dev/null
		@printf "%-15s ${_PURPLE}${_BOLD}${NAME}${_END}...\n" "Compiling"
		@$(CC) $(CFLAGS) ${OBJ} -Llibft -lft -L $(MAC_DIR_READLINE)/lib -lreadline -o $(NAME)
		@printf "\n${_GREEN}${_BOLD}[Compilation done !]${_END}\n"
else

$(OBJS_PATH)%.o: %.c $(HEADER)
		mkdir -p $(dir $@)
		@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
		@printf "%-15s ${_YELLOW}${_BOLD}$<${_END}...\n" "Compiling"
$(NAME): $(OBJ) ${HEADER} ${LIBFT}
		@printf "%-15s ${_CYAN}${_BOLD}libft${_END}...\n" "Compiling"
		@make -C ${LIBFT} > /dev/null
		@printf "%-15s ${_PURPLE}${_BOLD}${NAME}${_END}...\n" "Compiling"
		@$(CC) $(CFLAGS) ${OBJ} ./libft/libft.a -L/usr/local/lib -lreadline -o $(NAME)
		@printf "\n${_GREEN}${_BOLD}[Compilation done !]${_END}\n"
endif

clean:
	@printf "%-15s ${_RED}${_BOLD}${NAME}${_END}...\n" "Deleting"
	@rm -rf ${OBJS_PATH} 2> /dev/null
	@printf "%-15s ${_RED}${_BOLD}libft librairy${_END}...\n" "Deleting"
	@make clean -C $(LIBFT)

fclean: clean
	rm -f $(NAME)
	rm -f libft.a
	@make fclean -C $(LIBFT)

re: fclean all

run : $(NAME) ./.ignore_readline
	valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all --suppressions=.ignore_readline -q ./minishell

malloc_test: $(OBJ) ${HEADER} ${LIBFT}
	make -C ${LIBFT} > /dev/null
	$(CC) $(CFLAGS) -fsanitize=undefined -rdynamic -o $@ ${OBJ} -L./libft -lft -L/usr/local/lib -lreadline -L./ft_mallocator -lmallocator

.PHONY: all clean fclean bonus re run malloc_test
