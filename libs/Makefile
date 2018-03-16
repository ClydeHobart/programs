# Makefile for libs directory
# Builds and compiles utility functions into a library.
#
# Zeke Baker | 20180313.2120 | Programs/libs/Makefile

BITP = bitPrinting
HILB = hilbert
FILE = file

FLAGS = 
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS) # -g
CC = gcc
MAKE = make
OBJS = $(BITP).o $(HILB).o $(FILE).o
LIBS = libs.a

.PHONY: clean

$(LIBS): $(OBJS)
	ar cr $@ $^
	rm -f *.o

$(HILB).o: $(BITP).h $(HILB).h
	$(CC) $(CFLAGS) -c -o $@ $(HILB).c -lm

$(BITP).o: $(BITP).h

$(FILE).o: $(FILE).h

clean:
	rm -f *.o *.a *~  *.dSYM
