/* hilbert.c
 * A set of functions to do various things regarding the Hilbert curve. See
 * hilbert.h for more information regarding individual functions.
 *
 * Zeke Baker | 20180313.2140 | Programs/libs/hilbert.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bitPrinting.h"

double *hilbertMap(const char *bits);
int getExp(const char *doubleBits);
char *getFractionBits(const char *doubleBits);

int main()
{
	// double testDoubles[4];
	// int testExps[4];
	// char *testStrings[4];

	// testDoubles[0] = 0.045;
	// testDoubles[1] = 23;
	// testStrings[0] = printBEBits(testDoubles, sizeof(double), 1, 1);
	// testStrings[1] = printBEBits(&(testDoubles[1]), sizeof(double), 1, 1);
	// printf("%s\n%s\n", testStrings[0], testStrings[1]);
	// testExps[0] = getExp(testStrings[0]);
	// testExps[1] = getExp(testStrings[1]);
	// printf("%d\n%d\n", testExps[0], testExps[1]);

	// testStrings[2] = getFractionBits(testStrings[0]);
	// printf("%s\n", testStrings[2]);

	// srand(time(NULL));
	// for ( int i = 0; i < 40; i++ ) {
	// 	testDoubles[0] = ((double)rand()) / RAND_MAX;
	// 	testStrings[0] = printBEBits(testDoubles, sizeof(double), 1, 1);
	// 	testStrings[1] = getFractionBits(testStrings[0]);
	// 	printf("%8f | %64s | %d\n", testDoubles[0], testStrings[1], (int)strlen(testStrings[1]));
	// }

	for ( int i = 0; i < 8; i++ ) {
		double fraction = ((double)i) / 7;
		char *doubleBits = printBEBits(&fraction, sizeof(double), 1, 1);
		char *fractionBits = getFractionBits(doubleBits);

		double *pt = NULL;
		if ( fractionBits != NULL ) {
			if ( (int)strlen(fractionBits) % 2 == 1 ) {
				fractionBits[strlen(fractionBits) - 1] = 0;			}

			pt = hilbertMap(fractionBits);
		} else {
			pt = calloc(2, sizeof(double));
			pt[0] = -1;
			pt[1] = -1;
		}
		printf("%8f -> (%s%8f, %s%8f)\n", fraction, pt[0] >= 
			0 ? " " : "", pt[0], pt[1] >= 0 ? " " : "", pt[1]);
		free(pt);
		if ( fractionBits != NULL ) {
			free(fractionBits);
		}
		free(doubleBits);
	}

	return 0;
}

double *hilbertMap(const char *bits)
{
	double *pt, *retPt = calloc(2, sizeof(double));

	// check if no bits are left
	if ( strlen(bits) == 0 ) {
		retPt[0] = 0;
		retPt[1] = 0;
		// printf("\"\" -> (0, 0)\n");
		return(retPt);
	} else {
		pt = hilbertMap(bits + 2);
		switch (2 * bits[0] + bits[1] - 144) {
			case 0:
				retPt[0] = pt[1] * 0.5;
				retPt[1] = pt[0] * 0.5;
				break;
			case 1:
				retPt[0] = pt[0] * 0.5;
				retPt[1] = pt[1] * 0.5 + 0.5;
				break;
			case 2:
				retPt[0] = pt[0] * 0.5 + 0.5;
				retPt[1] = pt[1] * 0.5 + 0.5;
				break;
			case 3:
				retPt[0] = pt[1] * -0.5 + 1;
				retPt[1] = pt[0] * -0.5 + 0.5;
				break;
		}
		// printf("\"%c%c\" -> (%8f, %8f)\n", bits[0], bits[1], retPt[0], retPt[1]);
		free(pt);
		return(retPt);
	}
}

int getExp(const char *doubleBits)
{
	int exp = 0;
	// loop through the exponent bits of the double
	for ( int b = 1; b < 12; b++ ) {
		// if the bit is '1', add the appropriate amount to exp
		if ( doubleBits[b] == '1' ) {
			exp += pow(2, 11 - b);
		}
	}

	exp -= 1023;
	return exp;
}

char *getFractionBits(const char *doubleBits)
{
	int exp = -1 * getExp(doubleBits);	// exponent of the double
	int last1 = 11;											// index of last bit that is '1'
	int fBLen;													// length of fractionBits
	char *fractionBits;									// string of bits of the fraction

	// check if exp is < 1
	if ( exp < 1 ) {
		fractionBits = calloc(53, 1);
		strcat(fractionBits, "1111111111111111111111111111111111111111111111111111");
		return fractionBits;
	}

	// check if exp == 1023 (double was 0)
	if ( exp == 1023 ) {
		fractionBits = calloc(3, 1);
		strcat(fractionBits, "00");
	} else {
		// loop from back to front of doubleBits to find the last 1 bit
		for ( int b = 63; last1 == 11 && b > 11; b-- ) {
			// check if the bit is '1'
			if ( doubleBits[b] == '1' ) {
				last1 = b;
			}
		}

		fBLen = last1 + exp - 10;
		fractionBits = calloc(fBLen + 1, 1);

		// loop through new mantissa bits and set appropriately
		for ( int b = 0; b < exp; b++ ) {
			// check if it's the last new bit
			if ( b == exp - 1 ) {
				strcat(fractionBits, "1");
			} else {
				strcat(fractionBits, "0");
			}
		}

		strncat(fractionBits, doubleBits + 12, last1 - 11);
		strcat(fractionBits, "0");
	}
	return fractionBits;
}