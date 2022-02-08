# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/08 08:58:50 by ngiroux           #+#    #+#              #
#    Updated: 2022/01/09 15:51:17 by ngiroux          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Names 
NAME	= libunit.a
FILES	= internal/launcher.c internal/loader.c\
		pkg/lst_utils.c pkg/__putsr.c pkg/__putnbr.c\
		std/get_stdout.c\
		pkg/log.c\
		pkg/__start.c\
		pkg/malloc.c

# Paths
SRCS_PATH =	./srcs/
INCS_PATH = ./incs/
OBJS_PATH = ./objs/

LIBFT = ./libft

# Objects
SRCS = $(addprefix $(SRCS_PATH), $(FILES))
OBJS = $(addprefix $(OBJS_PATH), $(FILES:.c=.o))

# Flags + compilation
CFLAGS	= -Wall -Wextra -Werror
CC		= cc
IFLAGS	=  -I incs/ -I libft/includes

# Create obj and dir
$(OBJS_PATH)%.o: $(SRCS_PATH)%.c
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) -c $< -o $@ $(IFLAGS)

# Compiling command
all:		${NAME}

${NAME}:	${OBJS}
			@make -C ${LIBFT}
			@cp libft/libft.a ./${NAME}
			ar -rcs $(NAME) $(OBJS)

# Other rules
clean:
			rm -rf ${OBJS_PATH}

fclean:		clean
			rm -rf ${NAME}

re:			fclean all

.PHONY:		all clean fclean re