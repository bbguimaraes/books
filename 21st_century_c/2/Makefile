CFLAGS = -Wall -g -std=gnu11

all: gdb_test glist gprof_test sadstrings

test: test_gprof test_valgrind

glist: glist.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ \
		`pkg-config --cflags glib-2.0` $^ `pkg-config --libs glib-2.0`

gprof_test: gprof_test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -O0 -pg -o $@ $^

test_gprof:
	./gprof_test
	gprof gprof_test > profile
	cat profile

test_valgrind:
	valgrind --leak-check=full ./sadstrings

clean:
	rm -f gdb_test glist gprof_test profile gmon.out sadstrings
