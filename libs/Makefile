# Makefile for libs directory
# Builds and compiles utility functions into a library.
#
# Zeke Baker | 20180313.2120 | Programs/libs/Makefile

BITP = bitPrinting
HILB = hilbert

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make
OBJS = $(BITP).o $(HILB).o
LIB = libs.a

.PHONY: all clean

# $(LIB): $(OBJS)
# 	ar cr $(LIB) $(OBJS)

$(HILB): $(HILB).o $(BITP).o
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(HILB).o: $(BITP).h

$(BITP).o: $(BITP).h

clean:
	rm -f *~ *.o *.dSYM $(BITP) $(HILB)
