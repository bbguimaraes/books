CFLAGS = `pkg-config --cflags gsl` `pkg-config --cflags glib-2.0` -I../9
CFLAGS += -Wall -g -std=c11 -fms-extensions
LDLIBS = `pkg-config --libs gsl` `pkg-config --libs glib-2.0`

OBJECTS = \
	dict.a simple_cplx seamlessone seamlesstwo dict_use cetology \
	groupabm groupabm.pdf groups.pdf
all: $(OBJECTS)

dict.a: keyval.o dict.o
	ar rcs -o $@ $^

dict_use: keyval.o dict.o

simple_cplx: complex.o

moby:
	curl http://www.gutenberg.org/cache/epub/2701/pg2701.txt > moby

cetology: fstr.o ../9/string_utilities.o

../9/string_utilities.o:
	make -C ../9 string_utilities.o

groupabm: groupabm.c groups.o apop.c

groupabm.pdf: groupabm.tex
	pdftex $^

groups.pdf: groups.tex
	pdftex $^

test: moby
	./simple_cplx
	./seamlessone
	./seamlesstwo
	./dict_use
	./cetology

clean:
	rm -f \
		$(OBJECTS) keyval.o dict.o complex.o fstr.o moby \
		../9/string_utilities.o \
		groupabm{,.c,.idx,.log,.pdf,.scn,.tex,.toc} \
		groups{.c,.idx,.log,.o,.pdf,.scn,.tex,.toc}
