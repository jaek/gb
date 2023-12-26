CC=g++
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++20

SDIR=./src
DDIR=$(SDIR)
ODIR=$(SDIR)/obj


MAINFILE=cpu

_OBJ = registers.o cpu.o instructions.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEP = cpu.h registers.h 
DEP = $(patsubst %,$(DDIR)/%,$(_DEP))

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(MAINFILE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(ODIR) $(MAINFILE) 

tests: $(OBJ) $(SDIR)/tests.cpp
	$(CC) -g -o $@ $^ 

debug: test
	valgrind -q --vgdb-error=0 ./tests

test: CFLAGS += -Dunit_test
test: clean tests

.PHONY: clean test