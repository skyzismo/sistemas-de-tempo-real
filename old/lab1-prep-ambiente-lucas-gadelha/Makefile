# Makefile para compilar Matrix e Integral ADTs com nova estrutura

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -O0

# Diretórios
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj

# Nome do executável
TARGET = $(BIN_DIR)/main

# Arquivos fonte
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/Matrix.c $(SRC_DIR)/Integral.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Incluir paths
INCLUDES = -I$(INCLUDE_DIR)

# Regra padrão
all: $(TARGET)

# Criar diretórios se não existirem
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Compilação com otimização
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) -lm

# Compilação para debug
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(TARGET)

# Regras de compilação individuais
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INCLUDE_DIR)/Matrix.h $(INCLUDE_DIR)/Integral.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/Matrix.o: $(SRC_DIR)/Matrix.c $(INCLUDE_DIR)/Matrix.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/Integral.o: $(SRC_DIR)/Integral.c $(INCLUDE_DIR)/Integral.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR)

# Recompilar tudo
rebuild: clean all

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Executar com valgrind (para debug)
valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

.PHONY: all clean rebuild run debug valgrind