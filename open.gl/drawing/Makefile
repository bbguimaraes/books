CPPFLAGS = -std=c++11 -I..
all: colors drawing element uniforms
colors:   colors.cpp   ../common.h ../common.o -lglfw -lGLEW -lGLU -lGL
drawing:  drawing.cpp  ../common.h ../common.o -lglfw -lGLEW -lGLU -lGL
element:  element.cpp  ../common.h ../common.o -lglfw -lGLEW -lGLU -lGL
uniforms: uniforms.cpp ../common.h ../common.o -lglfw -lGLEW -lGLU -lGL
.PHONY: clean
clean:
	rm -f colors drawing element uniforms
