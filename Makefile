CC = gcc
PARAMS = -Wall -g

.PHONY: all clean

bin/action.o: source/action.c headers/networking.h
	$(CC) -o $@ -c $(PARAMS) $<

bin/main.o: main.c headers/networking.h
	$(CC) -o $@ -c $(PARAMS) $<

bin/networking.o: source/networking.c headers/networking.h
	$(CC) -o $@ -c $(PARAMS) $<

all: bin/main.o bin/networking.o bin/action.o
	$(CC) -o bin/final $(PARAMS) $^

clean:
	rm -rf bin/*

