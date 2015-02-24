CPPFLAGS = -std=c++11 -I..
COMMON = ../common.h ../common.o
LIBRARIES = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: depth depthstencils planar
depth:         depth.cpp         ${COMMON} ${LIBRARIES}
depthstencils: depthstencils.cpp ${COMMON} ${LIBRARIES}
planar: planar.cpp \
	../transformations_common.h ../transformations_common.o \
	${COMMON} ${LIBRARIES}
.PHONY: clean
clean:
	rm -f depth depthstencils planar
