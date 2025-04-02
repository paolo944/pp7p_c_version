CC = gcc
CFLAGS = -Wall -Wextra -g -Oz
SRC_DIR = src
ROUTES_DIR = routes
OBJ_DIR = obj
BIN = pp7p

SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(ROUTES_DIR)/*.c)
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

$(BIN): $(OBJ) main.c
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN)

fclean: clean

re: fclean $(BIN)

.PHONY: clean fclean re
