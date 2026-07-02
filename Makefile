CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = src/main.c src/server.c src/router.c src/map-factory.c \
      src/treemap.c src/list.c src/radix-tree.c
OUT = bin/servidor

$(OUT): $(SRC) | bin
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

bin:
	mkdir -p bin

clean:
	rm -rf bin

.PHONY: clean
