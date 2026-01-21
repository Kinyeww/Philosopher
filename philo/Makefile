NAME = philo
CC = cc
CCFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c parsing_utils.c eat_sleep_think.c
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