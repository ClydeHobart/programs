# Makefile for Programs directory
# Builds and compiles any non-modular programs into executables
#
# Zeke Baker | 20180316.1554 

FTS = fracToSphere
INV = inversions

L = libs

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS) # -g
CC = gcc
MAKE = make
LIBS = $L/$L.a

.PHONY: all clean

all: $(LIBS) $(FTS) $(INV)

$(LIBS):
	$(MAKE) -C $L

$(FTS): $(FTS).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(INV): $(INV).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o *~ *.dSYM $(FTS) $(INV)
	$(MAKE) -C $L clean