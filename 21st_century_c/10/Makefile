CFLAGS = -I../2 -I../9 `pkg-config --cflags glib-2.0` -Wall -g -std=gnu11
LDLIBS = -lm `pkg-config --libs glib-2.0`
OBJECTS = \
	 sum_to_nan \
	 stopif_test \
	 varad \
	 safe_sum \
	 vectorize \
	 boxes \
	 papersizes \
	 errortuple \
	 olden_varargs \
	 macro_varargs \
	 ideal \
	 amort_use \
	 show_tree \
	 charct
all: $(OBJECTS)

amort_use: amort_interface.o amortize.o
show_tree: process_dir.o
charct: unictr.o process_dir.o ../9/string_utilities.o
../9/string_utilities.o:
	make -C ../9 string_utilities.o

test:
	./sum_to_nan
	./stopif_test
	./varad
	./safe_sum
	./vectorize
	./boxes
	./papersizes
	./errortuple && exit 1 || echo ok
	./olden_varargs .
	./macro_varargs .
	./ideal
	./amort_use
	./show_tree ..
	./charct

clean:
	rm -f $(OBJECTS) amort_interface.o amortize.o process_dir.o unictr.o \
		../9/string_utilities.o
