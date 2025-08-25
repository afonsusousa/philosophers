NAME = philo
BONUS_NAME = philo_bonus_exec
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g

# Build directories
BUILD_DIR = build
PHILO_BUILD_DIR = $(BUILD_DIR)/philo
BONUS_BUILD_DIR = $(BUILD_DIR)/philo_bonus

# Source files
SRC = philo/philosophers.c philo/utils.c philo/actions.c philo/monitor.c philo/init_clean.c
BONUS_SRC = philo_bonus/philosophers.c philo_bonus/utils.c philo_bonus/actions.c philo_bonus/monitor.c philo_bonus/spawn.c philo_bonus/sems.c philo_bonus/cleanup.c

# Object files in build directory
OBJ = $(SRC:philo/%.c=$(PHILO_BUILD_DIR)/%.o)
BONUS_OBJ = $(BONUS_SRC:philo_bonus/%.c=$(BONUS_BUILD_DIR)/%.o)

# Headers
HEADER = philo/philosophers.h
BONUS_HEADER = philo_bonus/philosophers.h

all: $(NAME)

$(NAME): $(BUILD_DIR) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(BUILD_DIR) $(BONUS_OBJ)
	$(CC) $(CFLAGS) -o $(BONUS_NAME) $(BONUS_OBJ)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(PHILO_BUILD_DIR)
	mkdir -p $(BONUS_BUILD_DIR)

# Compile philo source files
$(PHILO_BUILD_DIR)/%.o: philo/%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile philo_bonus source files
$(BONUS_BUILD_DIR)/%.o: philo_bonus/%.c $(BONUS_HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

test: $(NAME)
	@echo "Testing with 4 philosophers, 800ms to die, 200ms to eat, 200ms to sleep:"
	./$(NAME) 4 800 200 200
	@echo "\nTesting with 5 philosophers, each must eat 3 times:"
	./$(NAME) 5 800 200 200 3

test_bonus: $(BONUS_NAME)
	@echo "Testing bonus with 4 philosophers, 800ms to die, 200ms to eat, 200ms to sleep:"
	./$(BONUS_NAME) 4 800 200 200
	@echo "\nTesting bonus with 5 philosophers, each must eat 3 times:"
	./$(BONUS_NAME) 5 800 200 200 3

.PHONY: all bonus clean fclean re test test_bonus
