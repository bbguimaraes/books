P = hello
OBJECTS =
CFLAGS = -g -Wall -O3
LBLIBS =
CC = c99

all: erf $(P) getenv

erf: erf.c
	# -g: add debbugin symbols.
	# -std=gnu11: use C11 and POSIX standars (gcc-specific).
	# -O3: optimization level three.
	# -Wall: add compile warnings.
	gcc erf.c -o erf -lm -g -Wall -O3 -std=gnu11

$(P): $(OBJECTS)

test: test_pkg-config test_getenv test_cmdline_compile

test_pkg-config:
	# pkg-config --libs gsl libxml-2.0
	# pkg-config --cflags gsl libxml-2.0

test_getenv: getenv
	./erf
	reps=10 msg="Ha" ./getenv
	msg="Ha" ./getenv
	reps=20 msg=" " ./getenv

test_cmdline_compile:
	echo 'int main() { printf("Hello from the command line.\n"); }' | \
		c99 -xc '-' -lm -g -Wall -include stdio.h
	./a.out

clean:
	rm -f erf hello getenv a.out
