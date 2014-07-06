CFLAGS = `pkg-config --cflags glib-2.0` -I../2 -Wall -g -std=gnu11
LDLIBS = `pkg-config --libs glib-2.0`

OBJECTS = \
	sadstrings \
	getstrings \
	saprintf \
	string_utilities \
	string_utilities.o \
	unicode

all: $(OBJECTS)

string_utilities: string_utilities.c
	$(CC) $(CFLAGS) -Dtest_ok_array $(LDFLAGS) -o $@ $^ $(LDLIBS)

unicode: unicode.c string_utilities.o

test:
	./sadstrings
	./getstrings
	./saprintf
	./string_utilities
	./unicode Makefile

clean:
	rm -f $(OBJECTS) uout.html
