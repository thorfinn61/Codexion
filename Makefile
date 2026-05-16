NAME		= codexion

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
INCLUDES	= -I include

OBJ_DIR		= obj

SRCS		= src/main.c \
			  src/parse.c \
			  src/init.c \
			  src/cleanup.c \
			  src/log.c \
			  src/dongle.c \
			  src/dongle_utils.c \
			  src/coder.c \
			  src/monitor.c \
			  src/utils/utils.c \
			  src/utils/utils_time.c \
			  src/pqueue/pqueue.c \
			  src/pqueue/pqueue_utils.c

OBJS		= $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))

HEADERS		= include/codexion.h

RM			= rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
