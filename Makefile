SHELL=/bin/bash

PRODUCT := portal.out

CXX := g++
LINKER := g++
INCDIRS := -I.
CXXFLAGS := -g -Wall -Wextra

SRCFILES := $(wildcard src/*.cpp) 
OBJFILES := $(patsubst %.cpp,%.o,$(SRCFILES))

$(PRODUCT): $(OBJFILES)
	$(LINKER) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

clean:
	rm -r src/*.o
	rm portal.out

