CFLAGS = -Wall -g -std=gnu11

OBJECTS = \
	fibo \
	copystructs \
	copystructs2 \
	automem \
	memcpy \
	arithmetic \
    pointer_arithmetic \
	pointer_arithmetic2

all: $(OBJECTS)

test:
	for x in $(OBJECTS); do ./$$x; done

clean:
	rm -f $(OBJECTS)
