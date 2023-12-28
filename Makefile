.DEFAULT_GOAL := all

SDIR=./src
IDIR=$(SDIR)
ODIR=./build

CC=g++ 
CFLAGS=-Wall -Wextra -Werror -pedantic -std=c++20 

_MAIN=cpu
MAIN=$(ODIR)/$(_MAIN)
TEST=$(ODIR)/test

_OBJ = registers.o cpu.o instructions.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_DEP = cpu.h registers.h instructions.h 
DEP = $(patsubst %,$(IDIR)/%,$(_DEP))

$(ODIR)/%.o: $(SDIR)/%.cpp
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJ)
	mkdir -p build
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(ODIR)/*

tests: $(OBJ) $(SDIR)/tests.cpp
	$(CC) -I $(IDIR) -g -o $(TEST) $^

debug: test
	valgrind -q --vgdb-error=0 $(TEST)

test: CFLAGS += -Dunit_test
test: clean tests

.PHONY: clean test all
all: $(MAIN)