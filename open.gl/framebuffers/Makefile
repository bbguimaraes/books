CPPFLAGS = -std=c++11 -I..
COMMON = \
	../common.h ../common.o \
	../framebuffers_common.h ../framebuffers_common.o
LIBRARIES = -lSOIL -lglfw -lGLEW -lGLU -lGL
all: blur framebuffers grayscale invert sobel weighted_gray ex1 ex2
blur:          blur.cpp          ${COMMON} ${LIBRARIES}
framebuffers:  framebuffers.cpp  ${COMMON} ${LIBRARIES}
invert:        invert.cpp        ${COMMON} ${LIBRARIES}
grayscale:     grayscale.cpp     ${COMMON} ${LIBRARIES}
sobel:         sobel.cpp         ${COMMON} ${LIBRARIES}
weighted_gray: weighted_gray.cpp ${COMMON} ${LIBRARIES}
ex1:           ex1.cpp           ${COMMON} ${LIBRARIES}
ex2:           ex2.cpp           ${COMMON} ${LIBRARIES}
.PHONY: clean
clean:
	rm -f blur framebuffers grayscale invert sobel weighted_gray ex1 ex2
