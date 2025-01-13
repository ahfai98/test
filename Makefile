# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/15 20:51:19 by jyap              #+#    #+#              #
#    Updated: 2025/01/13 18:31:29 by jyap             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC_DIR = src/

OBJ_DIR = obj/

INC_DIR = inc/

SRC_FILES = main.cpp \
			Utils.cpp \
			ServerManager.cpp \
			Response.cpp \
			Client.cpp \
			HttpRequest.cpp \
			ConfigParser.cpp \
			ServerConfig.cpp \
			Location.cpp \
			CgiHandler.cpp \
			Mime.cpp \
			Logger.cpp\

OBJ_FILES = $(SRC_FILES:.cpp=.o)

SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ			=	$(addprefix $(OBJ_DIR), $(OBJ_FILES))

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic
CFLAGS += -g3 

RM = rm -rf

RESET = "\033[0m"
BLACK = "\033[1m\033[37m"

all:
	@$(MAKE) $(NAME) -j5

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo $(BLACK)-webserv compiled.$(RESET)

$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -I $(INC_DIR) -o $@ -c $<

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: 	fclean all

.PHONY: all clean fclean re
