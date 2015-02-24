CPPFLAGS = -std=c++11 -I..
COMMON = \
    ../common.h ../common.o \
    ../transformations_common.h ../transformations_common.o
LIBRARIES = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: 3d ex1 ex2 transformations
3d:              3d.cpp              ${COMMON} ${LIBRARIES}
ex1:             ex1.cpp             ${COMMON} ${LIBRARIES}
ex2:             ex2.cpp             ${COMMON} ${LIBRARIES}
transformations: transformations.cpp ${COMMON} ${LIBRARIES}
.PHONY: clean
clean:
	rm -f 3d ex1 ex2 transformations
