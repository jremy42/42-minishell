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
gnl/get_next_line_utils.c

BONUS = list/ft_lstnew.c \
list/ft_lstadd_front.c \
list/ft_lstsize.c \
list/ft_lstlast.c \
list/ft_lstadd_back.c \
list/ft_lstdelone.c \
list/ft_lstclear.c \
list/ft_lstiter.c \
list/ft_lstmap.c

CFLAGS = -Wall -Wextra -Werror
IFLAGS = -I includes/
HEADER = includes/libft.h
NAME = libft.a
CC = gcc
OBJ = $(SRC:.c=.o)
OBJBONUS = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(HEADER)
		ar rc $(NAME) $(OBJ)

bonus : $(OBJ) $(OBJBONUS)
		ar rc $(NAME) $(OBJ) $(OBJBONUS)
%.o:%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJBONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re