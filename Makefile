all: test-fit

GEOM=../libgeom

INCLUDES=-I$(GEOM) -I/usr/include/eigen3
LIBS=-L$(GEOM)/release -lgeom -lasan

CXXFLAGS=-std=c++20 -Wall -pedantic -O3 -DNDEBUG $(INCLUDES)
#CXXFLAGS=-std=c++20 -Wall -pedantic -O0 -g -DDEBUG $(INCLUDES) -fsanitize=address

libconic.a: conic-fit.o fitter.o solver.o
	$(AR) rcs $@ $^

test-fit: test-fit.o libconic.a
	$(CXX) -o $@ $< -L. -lconic $(LIBS)
