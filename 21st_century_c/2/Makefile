CFLAGS = -Wall -g -std=gnu11

all: gdb_test glist gprof_test sadstrings dict_test
.PHONY: ../11/dict.a

test: test_gprof test_valgrind test_dict

gdb_test: gdb_test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -w -o $@ $^

glist: glist.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ \
		`pkg-config --cflags glib-2.0` $^ `pkg-config --libs glib-2.0`

gprof_test: gprof_test.c
	$(CC) $(CFLAGS) $(LDFLAGS) -O0 -pg -o $@ $^

dict_test: dict_test.c ../11/dict.a
	$(CC) $(CFLAGS) $(LDFLAGS) -I../11 `pkg-config --cflags glib-2.0`\
		-O0 -fprofile-arcs -ftest-coverage \
		-o $@ dict_test.c ../11/dict.a `pkg-config --libs glib-2.0`

../11/dict.a:
	$(MAKE) -C ../11 dict.a

test_gprof:
	./gprof_test
	gprof gprof_test > profile
	cat profile

test_valgrind:
	valgrind --leak-check=full ./sadstrings

test_dict:
	./dict_test
	gcov dict_test
	cat dict_test.c.gcov

clean:
	rm -f gdb_test glist gprof_test profile gmon.out sadstrings dict_test
	rm -f dict_test.gcda dict_test.gcno dict_test.c.gcov
	make -C ../11 clean
