NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g
SRC = philosophers.c utils.c actions.c monitor.c init_clean.c
OBJ = $(SRC:.c=.o)
HEADER = philosophers.h

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: $(NAME)
	@echo "Testing with 4 philosophers, 800ms to die, 200ms to eat, 200ms to sleep:"
	./$(NAME) 4 800 200 200
	@echo "\nTesting with 5 philosophers, each must eat 3 times:"
	./$(NAME) 5 800 200 200 3

.PHONY: all clean fclean re test
