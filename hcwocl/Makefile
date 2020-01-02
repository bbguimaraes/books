CPPFLAGS = -I . -I TestBed/ -DCOMPUTE_DEVICE_CL -DCL_TARGET_OPENCL_VERSION=200
CXXFLAGS = -std=c++20 -O2
LDLIBS = -lglut -lGLU -lGL -lOpenCL -lpthread -lm

OBJECTS = \
	TestBed/main.o \
	TestBed/Demos/Dem2Demo.o \
	Common/Base/ThreadPool.o \
	Common/Math/RadixSortHost.o

.PHONY: all clean
all: TestBed/TestBed

clean:
	rm -f $(OBJECTS)

TestBed/TestBed: $(OBJECTS)
	$(CXX) -o $@ $^ $(LDLIBS)
