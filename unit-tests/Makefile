# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jremy <jremy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/08 10:12:53 by ngiroux           #+#    #+#              #
#    Updated: 2022/02/08 11:48:17 by jremy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Define
DEF			= FT_TESTED='"1"'\
			FT00_TEST='"1111"' 
DEFS		= $(addprefix -D, $(DEF))

# Names 
NAME		= tester
LIB_NAME	= libunit.a
FILES		= ../srcs/bin/pwd/pwd.c\
			../srcs/bin/echo/echo.c\
			00_pwd/00_launcher.c\
			00_pwd/01_basic_pwd.c\
			01_echo/05_NULL_echo.c\
			01_echo/02_flag_echo.c\
			01_echo/04_invalid_flag_echo.c\
			01_echo/06_NULL_f_echo.c\
			01_echo/01_basic_echo.c\
			01_echo/00_launcher2.c\
			01_echo/03_multi-flag_echo.c\
			main.c

# Paths
LIB_PATH		=framework/
INCS_PATH		=./incs/
LIB_INCS_PATH	=$(LIB_PATH)$(INCS_PATH)
OBJS_PATH		=./objs/
MINISHELL_PATH	= ../includes/
LIBFT_PATH	= ../libft/includes/


# Objects
OBJS = $(addprefix $(OBJS_PATH), $(FILES:.c=.o))

# Flags + compilation
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
CFLAGS	+= $(DEFS)
LIB_CC	= -L$(LIB_PATH) -lunit
INCS	= -I $(LIB_INCS_PATH) -I $(INCS_PATH) -I $(INCS_PATH) -I $(MINISHELL_PATH) -I $(LIBFT_PATH)

# Create obj and dir
$(OBJS_PATH)%.o: %.c
			mkdir -p $(dir $@)
			$(CC) $(CFLAGS) $(INCS) -c $< -o $@  

# Compiling command
all:		${NAME}

${NAME}:	${OBJS}
			$(CC) $(CFLAGS) $(OBJS) $(LIB_CC) -o $(NAME)

test:		${NAME}
			./${NAME} || true
# Other rules
clean:
			rm -rf ${OBJS_PATH}
			make -C $(LIB_PATH) clean

fclean:		clean
			rm -rf ${NAME}
			rm -rf ${LIB_NAME}
			make -C $(LIB_PATH) fclean

re:			fclean all

.PHONY:		all clean fclean re test
