# Makefile for Programs directory
# Builds and compiles any non-modular programs into executables
#
# Zeke Baker | 20180316.1554 

FTS = fracToSphere
INV = inversions
KMP = karatMultPoly
SQS = squareSummable
LIS = longIncSubseq
PS4 = ps4

ALL = $(FTS) $(INV) $(KMP) $(SQS) $(LIS) $(PS4)

L = libs

FLAGS =
CFLAGS = -Wall -pedantic -std=c11 $(FLAGS) -g
CC = gcc
MAKE = make
LIBS = $L/$L.a

.PHONY: all clean

all: $(LIBS) $(ALL)

$(LIBS):
	$(MAKE) -C $L

$(FTS): $(FTS).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(INV): $(INV).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(KMP): $(KMP).c
	$(CC) $(CFLAGS) $^ -o $@

$(SQS): $(SQS).c
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(LIS): $(LIS).c $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

$(PS4): $(PS4).c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o *~ *.dSYM $(ALL)
	$(MAKE) -C $L clean