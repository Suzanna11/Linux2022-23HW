CC=g++
CFLAGS=-Wall -Wextra -pedantic -std=c++11

all: init destroy print set

init: init.cpp
	$(CC) $(CFLAGS) -o init init.cpp -lrt

destroy: destroy.cpp
	$(CC) $(CFLAGS) -o destroy destroy.cpp -lrt

print: print.cpp
	$(CC) $(CFLAGS) -o print print.cpp -lrt

set: set.cpp
	$(CC) $(CFLAGS) -o set set.cpp -lrt

clean:
	rm -f init destroy print set

