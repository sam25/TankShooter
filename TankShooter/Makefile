# variables
EXE=shooter5
CC=g++
CFLAGS=-ggdb -Wall -std=c++98 -pedantic $(shell sdl-config --cflags)
CXX=$(CC)
CXXFLAGS=$(CFLAGS)
VALGRIND_CL=
#-W(warnings) 
#-pedantic ( conform to standards!!! )
#-std=c99 means use the c99 standard
LFLAGS=$(shell sdl-config --libs) -lSDL_image -lSDL_gfx -lm -lSDL_ttf -lSDL_net
SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
DEPS:=$(patsubst %.cpp,%.d,$(SRCS))

# link step
$(EXE): $(DEPS) $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LFLAGS)

# $< means the %.c  
%.d: %.cpp
	$(CC) $(CFLAGS) -M $< > $@
	$(CC) $(CFLAGS) -M $< | sed s/\\.o/.d/ >> $@

clean:
	rm -f $(OBJS) $(DEPS) 

valgrind:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./$(EXE) $(VALGRIND_CL)

-include $(DEPS)
