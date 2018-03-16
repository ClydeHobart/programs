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
#include "bitPrinting.h"

/* **************** global functions ****************
 * See hilbert.h for function descriptions
 */

/**************** hilbertMap() ****************/
double *hilbertMap(const double fraction);

/**************** local functions ****************/
static double *hilbertMapHelp(const char *bits);
static int getExp(const char *doubleBits);
static char *getFractionBits(const char *doubleBits);

/**************** hilbertMap() ****************/
double *hilbertMap(const double fraction)
{
	double *pt;					// double ptr for the coordinates of the hilbert mapping
	char *doubleBits;		// string for the 64 bits of the double fraction
	char *fractionBits;	// string for the bits the fraction bits
		// (adjusted by the proper exponent)

	doubleBits = printBEBits((void *)&fraction, sizeof(double), 1, 1);
	fractionBits = getFractionBits(doubleBits);
	pt = NULL;

	// check if fractionBits is NULL (the exponent bits are too high)
	if ( fractionBits != NULL ) {
		// check if there's an odd amount of fraction bits
		if ( (int)strlen(fractionBits) % 2 == 1 ) {
			fractionBits[strlen(fractionBits) - 1] = 0;
		}

		pt = hilbertMapHelp(fractionBits);
		free(fractionBits);
	} else {
		pt = calloc(2, sizeof(double));
		pt[0] = 1;
		pt[1] = 0;
	}

	free(doubleBits);
	return pt;
}

/* **************** hilbertMapHelp() ****************
 * maps a string of fraction bits to a coordinate (x, y), where x and y are in 
 * [0, 1], on the Hilbert curve
 *
 * bits: string of even-numbered fraction bits to find the Hilbert mapping of
 * return: double ptr to two consecutive doubles for the x and y coordinate
 *
 * memory contract: the returned ptr needs to be freed
 */
static double *hilbertMapHelp(const char *bits)
{
	double *pt, *retPt = calloc(2, sizeof(double));

	// check if no bits are left
	if ( strlen(bits) == 0 ) {
		retPt[0] = 0;
		retPt[1] = 0;
		// printf("\"\" -> (0, 0)\n");
		return(retPt);
	} else {
		pt = hilbertMapHelp(bits + 2);
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

/* **************** getExp() ****************
 * gets an integer of the exponent of the bits of a double
 *
 * doubleBits: string of the 64 bits of a double to find the exponent of
 * return: integer of the exponent of the double, adjusted by 1023
 */
static int getExp(const char *doubleBits)
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

/* **************** getFractionBits() ****************
 * gets a string of the fraction bits of a double, adjusted by the proper exponent
 *
 * doubleBits: string of the 64 bits of a double to find the fraction bits of
 * return: string of the fraction bits of doubleBits, adjusted by the proper exponent
 *
 * memory contract: the retuned ptr needs to be freed
 */
static char *getFractionBits(const char *doubleBits)
{
	int exp = -1 * getExp(doubleBits);	// exponent of the double
	int last1 = 11;											// index of last bit that is '1'
	int fBLen;													// length of fractionBits
	char *fractionBits;									// string of bits of the fraction

	// check if exp is < 1
	if ( exp < 1 ) {
		return NULL;
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