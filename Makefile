LIBDIRS= -framework GLUT -framework OpenGL
LDLIBS = -lobjc -lm -lpng -lz

CPPFLAGS= -Wno-deprecated
LDFLAGS= $(LIBDIRS)

TARGETS = pacman

SRCS = pacman.cpp

OBJS =  $(SRCS:.cpp=.o)

CXX = g++

default: $(TARGETS)
