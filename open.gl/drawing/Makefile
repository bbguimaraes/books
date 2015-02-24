CPPFLAGS = -std=c++11 -I..
COMMON = ../common.h ../common.o
LIBRARIES = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: colors drawing element ex1 ex2 ex3 uniforms
colors:   colors.cpp   ${COMMON} ${LIBRARIES}
drawing:  drawing.cpp  ${COMMON} ${LIBRARIES}
element:  element.cpp  ${COMMON} ${LIBRARIES}
ex1:      ex1.cpp      ${COMMON} ${LIBRARIES}
ex2:      ex2.cpp      ${COMMON} ${LIBRARIES}
ex3:      ex3.cpp      ${COMMON} ${LIBRARIES}
uniforms: uniforms.cpp ${COMMON} ${LIBRARIES}
.PHONY: clean
clean:
	rm -f colors drawing element ex1 ex2 ex3 uniforms
