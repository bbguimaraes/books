CFLAGS = -Wall -g -std=gnu11

all: dict.a keyval.o dict.o

dict.a: keyval.o dict.o
	ar rcs -o $@ $^

clean:
	rm -f dict.a keyval.o dict.o
