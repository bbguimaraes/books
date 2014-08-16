CFLAGS = `pkg-config --cflags glib-2.0` `curl-config --cflags` -I../2 -I../9
CFLAGS += -I/usr/include/libxml2 -Wall -g -std=c11
LDFLAGS = `pkg-config --libs glib-2.0` `curl-config --libs` -pthread -lxml2

OBJECTS = mmap pthreads mutex_wc gsl_distance nyt_feed

all: $(OBJECTS)

pthreads: ../9/string_utilities.o

mutex_wc: ../9/string_utilities.o

../9/string_utilities.o:
	make -C ../9 string_utilities.o

gsl_distance: gsl_distance.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lapophenia

test:
	./mmap
	./pthreads pthreads.c mutex_wc.c
	./mutex_wc pthreads.c mutex_wc.c
	./gsl_distance
	./nyt_feed

clean:
	rm -f \
		$(OBJECTS) mmapped.bin ../9/string_utilities.o \
		now.html nytimes_feeds.rss
