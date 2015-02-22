CPPFLAGS = -std=c++11 -I..
all: drawing uniforms
drawing: drawing.cpp ../common.h ../common.o -lglfw -lGLEW -lGLU -lGL
uniforms: uniforms.cpp ../common.h ../common.o -lglfw -lGLEW -lGLU -lGL
.PHONY: clean
clean:
	rm -f drawing uniforms
