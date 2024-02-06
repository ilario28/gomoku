##
## EPITECH PROJECT, 2022
## Makefile
## File description:
## Makefile
##

NAME	= pbrain-gomoku-ai

SRC		= 	src/Gomoku.cpp	\
			src/main.cpp	\

OBJ		= $(SRC:.cpp=.o)

CC 		= g++

CPPFLAGS	= -W -Wall -Wextra -Werror -g -std=c++20 -I include/

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
		rm -f $(OBJ)

fclean:		clean
		rm -f $(NAME)

re:		clean fclean all

.PHONY:	all clean fclean re
