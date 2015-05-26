CPPFLAGS = -std=c++11 -I..
OBJECTS = \
	common.o \
	transformations_common.o \
	framebuffers_common.o \
	geometry_common.o
all: $(OBJECTS)
common.o: common.h common.cpp
.PHONY: clean
clean:
	rm -f $(OBJECTS)
