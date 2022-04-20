SRC = string/ft_isalpha.c \
string/ft_isdigit.c \
string/ft_isalnum.c \
string/ft_isascii.c \
string/ft_isprint.c \
string/ft_strlen.c \
mem/ft_memset.c \
mem/ft_bzero.c \
mem/ft_memcpy.c \
mem/ft_memmove.c \
string/ft_strlcpy.c \
string/ft_strlcat.c \
string/ft_tolower.c \
string/ft_toupper.c \
string/ft_strchr.c \
string/ft_strrchr.c \
string/ft_strncmp.c \
mem/ft_memchr.c \
mem/ft_memcmp.c \
string/ft_strnstr.c \
stdlib/ft_atoi.c \
mem/ft_calloc.c \
string/ft_strdup.c \
string/ft_substr.c \
string/ft_strjoin.c \
string/ft_strtrim.c \
string/ft_split.c \
stdlib/ft_itoa.c \
string/ft_strmapi.c \
string/ft_striteri.c \
print/ft_putchar_fd.c \
print/ft_putstr_fd.c \
print/ft_putendl_fd.c \
print/ft_putnbr_fd.c \
gnl/get_next_line.c \
gnl/get_next_line_utils.c\
list/ft_lstnew.c \
list/ft_lstadd_front.c \
list/ft_lstsize.c \
list/ft_lstlast.c \
list/ft_lstadd_back.c \
list/ft_lstdelone.c \
list/ft_lstclear.c \
list/ft_lstiter.c \
list/ft_lstmap.c\
garbage_collector/free.c\
garbage_collector/malloc.c

_GREY=	$'\033[30m
_RED=	$'\033[31m
_GREEN=	$'\033[32m
_YELLOW=$'\033[33m
_BLUE=	$'\033[34m
_PURPLE=$'\033[35m
_CYAN=	$'\033[36m
_WHITE=	$'\033[37m
_END= $'\033[37m

CFLAGS = -Wall -Wextra -Werror
IFLAGS = -I includes/
HEADER = includes/libft.h
NAME = libft.a
CC = gcc
OBJ = $(SRC:.c=.o)
OBJBONUS = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
		@printf "%-15s ${_PURPLE}${_BOLD}${NAME}${_END}...\n" "Compiling"
		ar rc $(NAME) $(OBJ)
		@printf "\n${_GREEN}${_BOLD}Compilation done !${_END}\n"

%.o:%.c
	@$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
	@printf "%-15s ${_YELLOW}${_BOLD}$<${_END}...\n" "Compiling"


clean:
	rm -f $(OBJ) $(OBJBONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
