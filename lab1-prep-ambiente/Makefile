CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -pedantic -O2 -lm
TARGET = math_app
OBJS = Matrix.o Integral.o interface.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm

Matrix.o: Matrix.c Matrix.h
	$(CC) $(CFLAGS) -c Matrix.c

Integral.o: Integral.c Integral.h
	$(CC) $(CFLAGS) -c Integral.c

main.o: main.c Matrix.h Integral.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run