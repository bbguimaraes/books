CPPFLAGS = -std=c++11 -I..
COMMON = ../common.h ../common.o
LIBRARIES = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: ex1 ex2 ex3 textures  units
ex1:      ex1.cpp      ${COMMON} ${LIBRARIES}
ex2:      ex2.cpp      ${COMMON} ${LIBRARIES}
ex3:      ex3.cpp      ${COMMON} ${LIBRARIES}
textures: textures.cpp ${COMMON} ${LIBRARIES}
units:    units.cpp    ${COMMON} ${LIBRARIES}
.PHONY: clean
clean:
	rm -f ex1 ex2 ex3 textures units
