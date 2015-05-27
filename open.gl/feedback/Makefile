CPPFLAGS = -std=c++11 -I..
LDFLAGS = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: feedback geometry variable ex1
feedback: feedback.cpp ../common.o
geometry: geometry.cpp ../common.o
variable: variable.cpp ../common.o
ex1:      ex1.cpp      ../common.o
.PHONY: clean
clean:
	rm -f feedback geometry variable ex1
