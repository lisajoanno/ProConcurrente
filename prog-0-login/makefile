CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXEC_NAME = main
OBJ_FILES = temp.o

# Repertoires
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin

all : create_bin $(EXEC_NAME)

clean : 
	rm -rf $(EXEC_NAME) $(BIN_DIR)

create_bin : 
	mkdir -p $(BIN_DIR)

$(EXEC_NAME) : $(BIN_DIR)/$(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(BIN_DIR)/$(OBJ_FILES)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

