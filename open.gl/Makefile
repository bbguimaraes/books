CPPFLAGS = -std=c++11 -I..
all: common.o texture_common.o
common.o: common.h common.cpp
texture_common.o: texture_common.h texture_common.cpp
.PHONY: clean
clean:
	rm -f common.o texture_common.o
