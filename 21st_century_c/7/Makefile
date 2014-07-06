CFLAGS = -Wall -g -std=gnu11
LDFLAGS = -lm
OBJECTS = goto na floatfail stddev
all: $(OBJECTS)
test:
	for x in $(OBJECTS); do ./$$x; done
clean:
	rm -f $(OBJECTS)
