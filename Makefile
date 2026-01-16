# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -Icommunication -g

# Targets
CLIENT_TARGET = dCacheClient
SERVER_TARGET = dCacheServer

# Shared Files
SHARED_SRC = communication/message.c
SHARED_OBJ = $(SHARED_SRC:.c=.o)

# Client Files
CLIENT_SRC = dCacheClient.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# Server Files
SERVER_SRC = dCacheServer.c
SERVER_OBJ = $(SERVER_SRC:.c=.o)

# Default rule: build both
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Linking Client: Needs its own .o and the shared .o
$(CLIENT_TARGET): $(CLIENT_OBJ) $(SHARED_OBJ)
	$(CC) $(CLIENT_OBJ) $(SHARED_OBJ) -o $(CLIENT_TARGET)

# Linking Server: Needs its own .o and the shared .o
$(SERVER_TARGET): $(SERVER_OBJ) $(SHARED_OBJ)
	$(CC) $(SERVER_OBJ) $(SHARED_OBJ) -o $(SERVER_TARGET)

# Generic rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f *.o communication/*.o $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all clean