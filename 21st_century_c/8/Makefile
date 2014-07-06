CFLAGS = -Wall -g -std=gnu11
OBJECTS = \
	curly \
	sizeof \
	preprocess \
	constchange \
	conststruct \
	iggy_pop_detector \
	constfusion
all: $(OBJECTS)
test:
	for x in $(OBJECTS); do ./$$x; done
	./iggy_pop_detector Iggy Pop; echo $$?
	./iggy_pop_detector Billy o cão; echo $$?

clean:
	rm -f $(OBJECTS)
