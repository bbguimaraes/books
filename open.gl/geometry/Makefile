CPPFLAGS = -std=c++11 -I..
COMMON = ../common.h ../common.o ../geometry_common.h ../geometry_common.o
LIBRARIES = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: geometry ex1 vertex_attrs dynamic ex2
geometry:     geometry.cpp     ${COMMON} ${LIBRARIES}
ex1:          ex1.cpp          ${COMMON} ${LIBRARIES}
vertex_attrs: vertex_attrs.cpp ${COMMON} ${LIBRARIES}
dynamic:      dynamic.cpp      ${COMMON} ${LIBRARIES}
ex2:          ex2.cpp          ${COMMON} ${LIBRARIES}
.PHONY: clean
clean:
	rm -f geometry ex1 vertex_attrs dynamic ex2
