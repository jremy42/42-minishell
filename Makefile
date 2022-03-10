SRC = srcs/minishell.c \
srcs/bin/pwd/pwd.c\
srcs/bin/echo/echo.c\
srcs/bin/cd/cd.c\
srcs/bin/env/env.c\
srcs/bin/export/export.c\
srcs/bin/unset/unset.c\
srcs/signal.c\
srcs/exit.c\
srcs/bin/export/export_utils.c\
srcs/tokenizer/tokenizer_utils.c\
srcs/tokenizer/tokenizer_utils2.c\
srcs/lexer/lexer.c\
srcs/tokenizer/tokenizer.c\
srcs/syntax/syntax_checker.c\
srcs/tree/create_tree.c\
srcs/exe/exe_childs.c\
srcs/exe/init_seq.c\
srcs/exe/create_cmd.c\
srcs/exe/create_redirect.c\
srcs/exe/launcher_forks.c\
srcs/exe/exe.c


IFLAGS =  -I includes/ -I libft/includes
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
#CFLAGS = -Wall -Wextra -Werror
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
_BLUE=	$'\033[34mint __synthax_checker(t_lexing *lexing)

_PURPLE=$'\033[35m
_CYAN=	$'\033[36m
_WHITE=	$'\033[37m
_END= $'\033[37m

all: $(NAME)

ifeq ($(shell uname -s), Darwin)

$(OBJS_PATH)%.o: %.c $(HEADER)
		mkdir -p $(dir $@)
		@$(CC) $(CFLAGS) $(IFLAGS) -c $< -I /opt/homebrew/opt/readline/include -o $@
		@printf "%-15s ${_YELLOW}${_BOLD}$<${_END}...\n" "Compiling"

$(NAME): $(OBJ) ${HEADER} ${LIBFT}
		@printf "%-15s ${_CYAN}${_BOLD}libft${_END}...\n" "Compiling"
		@make -C ${LIBFT} > /dev/null
		@printf "%-15s ${_PURPLE}${_BOLD}${NAME}${_END}...\n" "Compiling"
		@$(CC) $(CFLAGS) ${OBJ} -Llibft -lft -L /opt/homebrew/opt/readline/lib -lreadline -o $(NAME)
		@printf "\n${_GREEN}${_BOLD}[Compilation done !]${_END}\n"

else

$(OBJS_PATH)%.o: %.c $(HEADER)
		mkdir -p $(dir $@)
		@$(CC) $(CFLAGS) $(IFLAGS) -c $< -I .brew/opt/readline/include -o $@
		@printf "%-15s ${_YELLOW}${_BOLD}$<${_END}...\n" "Compiling"
$(NAME): $(OBJ) ${HEADER} ${LIBFT}
		@printf "%-15s ${_CYAN}${_BOLD}libft${_END}...\n" "Compiling"
		@make -C ${LIBFT} > /dev/null
		@printf "%-15s ${_PURPLE}${_BOLD}${NAME}${_END}...\n" "Compiling"
		@$(CC) $(CFLAGS) ${OBJ} -Llibft -lft -L/usr/local/lib -I/usr/local/include -lreadline -o $(NAME)
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

.PHONY: all clean fclean bonus re run
