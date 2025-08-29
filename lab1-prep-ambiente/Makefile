# Makefile para compilar Matrix e Integral ADTs

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2 -lm
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -O0 -lm

# Nome do executável
TARGET = main

# Arquivos fonte
SRCS = main.c Matrix.c Integral.c
OBJS = $(SRCS:.c=.o)

# Regra padrão
all: $(TARGET)

# Compilação com otimização
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CFLAGS)

# Compilação para debug
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(TARGET)

# Regras de compilação individuais
main.o: main.c Matrix.h Integral.h
	$(CC) $(CFLAGS) -c main.c

Matrix.o: Matrix.c Matrix.h
	$(CC) $(CFLAGS) -c Matrix.c

Integral.o: Integral.c Integral.h
	$(CC) $(CFLAGS) -c Integral.c

# Limpeza
clean:
	rm -f $(OBJS) $(TARGET)

# Recompilar tudo
rebuild: clean all

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Executar com valgrind (para debug)
valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

.PHONY: all clean rebuild run debug valgrind