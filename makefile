CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXEC_NAME = main
INCLUDES =
LIBS =
OBJ_FILES = main.o #temp.o

all : $(EXEC_NAME)

clean : 
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<
