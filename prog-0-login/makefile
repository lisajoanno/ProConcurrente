CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXEC_NAME = main
# OBJ_FILES = diffusion_chaleur.o utils.o main.o 

# # Repertoires
OBJ_DIR = obj
SRC_DIR = src
BIN_DIR = bin

all : create_bin $(EXEC_NAME)

clean : 
	rm -rf $(EXEC_NAME) $(BIN_DIR)

create_bin : 
	mkdir -p $(BIN_DIR)

$(EXEC_NAME) : $(BIN_DIR)/main.o $(BIN_DIR)/diffusion_chaleur.o $(BIN_DIR)/utils.o
	$(CC) -o $(EXEC_NAME) $(BIN_DIR)/main.o $(BIN_DIR)/diffusion_chaleur.o $(BIN_DIR)/utils.o 

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
