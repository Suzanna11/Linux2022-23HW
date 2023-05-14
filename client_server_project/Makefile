CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++11

all: client server

client: client.cpp utils.h
	$(CC) $(CFLAGS) -o client client.cpp -lrt

server: server.cpp utils.h
	$(CC) $(CFLAGS) -o server server.cpp -lrt

clean:
	rm -f client server

