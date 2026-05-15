NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INCLUDES	= -I include

SRC_DIR		= src
OBJ_DIR		= obj

SRCS		= main.c \
			  parse.c \
			  utils.c \
			  pqueue.c \
			  pqueue_utils.c \
			  init.c \
			  cleanup.c

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

HEADERS		= include/codexion.h

RM			= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
