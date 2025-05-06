CC = gcc
CFLAGS = -Wall -I include
SRC = src
OBJ = main.o httpd.o sql.o repository.o json.o
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: $(SRC)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o $(TARGET)