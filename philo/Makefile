NAME = philo
CC = cc
CCFLAGS = -Wall -Wextra -Werror -g -I./includes -pthread #-fsanitize=thread

SRCS = srcs/main.c srcs/parsing_utils.c srcs/actions.c srcs/routine.c srcs/routine_helper.c srcs/init_clean.c
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

me: re
	rm -f $(OBJS)

.PHONY: all clean fclean re me
