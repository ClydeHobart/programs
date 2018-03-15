/* bitPrinting.c
 * A set of functions to do various things regarding bits. See bitPrinting.h for 
 * more information regarding individual functions.
 *
 * Zeke Baker | 20180313.2135 | Programs/libs/bitPrinting.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

char *printBEBits(void *ptr, const int size, const int count, const char retStr);
char *printLEBits(void *ptr, const int size, const int count, const char retStr);
void printByte(void *ptr, char *dest);
void invertBytes(void *ptr, const int count);

// int main()
// {
// 	char word[4] = { 0, 1, 2, 3 };
// 	double *pi = malloc(sizeof(double));
// 	*pi = M_PI;
// 	int *num = malloc(sizeof(int));
// 	*num = 1;

// 	printBEBits(pi, sizeof(double), 1, 0);
// 	printBEBits(pi, sizeof(double), 1, 0);
// 	printLEBits(pi, sizeof(double), 1, 0);
// 	puts("");
// 	printBEBits(pi, sizeof(double), 1, 0);
// 	puts("inverting");
// 	invertBytes(pi, sizeof(double));
// 	printBEBits(pi, sizeof(double), 1, 0);
// 	puts("inverting");
// 	invertBytes(pi, sizeof(double));
// 	printBEBits(pi, sizeof(double), 1, 0);
// 	puts("");
// 	char *piStr = printBEBits(pi, sizeof(double), 1, 1);
// 	printf("%s\n\n", piStr);
// 	printBEBits(num, sizeof(int), 1, 0);
// 	printLEBits(num, sizeof(int), 1, 0);
// 	printBEBits(word, sizeof(char), 4, 0);
// 	printLEBits(word, sizeof(char), 4, 0);
// 	free(pi);
// 	free(num);
// 	free(piStr);
// }

/**************** isBigEndian() ****************/
char isBigEndian() {
	int i = 1;
	return ((unsigned char *)&i)[0] == '\0';
}

/**************** printBEBits() ****************/
char *printBEBits(void *ptr, const int size, const int count, const char retStr)
{
	char *bitString = NULL;
	char iBE = isBigEndian(); // boolean for whether or not host is big-endian

	// allocate memory if return string is necessary
	if ( retStr ) {
		bitString = calloc(count * size * 8 + 1, sizeof(char));

		// check if bitString was allocated properly
		if ( bitString == NULL ) {
			fprintf(stderr, "Error: difficulty allocating memory for big-endian bitString\n");
			return NULL;
		}
	}

	for ( int e = 0; e < count; e++ ) {
		for ( int B = iBE ? 0 : size - 1;
			( iBE && B < size ) || B >= 0;
			B = iBE ? B + 1 : B - 1) {
			printByte(((char *)ptr) + e * size + B, bitString);
		}
	}

	// print newline character if no string to be returned
	if ( ! retStr ) {
		puts("");
	}

	return bitString;
}

/**************** printLEBits() ****************/
char *printLEBits(void *ptr, const int size, const int count, const char retStr)
{
	char *bitString = NULL;
	char iBE = isBigEndian(); // boolean for whether or not host is big-endian

	// allocate memory if return string is necessary
	if ( retStr ) {
		bitString = calloc(count * size * 8 + 1, sizeof(char));

		// check if bitString was allocated properly
		if ( bitString == NULL ) {
			fprintf(stderr, "Error: difficulty allocating memory for little-endian bitString\n");
			return NULL;
		}
	}

	for ( int e = 0; e < count; e++ ) {
		for ( int B = iBE ? size - 1 : 0;
			( iBE && B >= 0 ) || B < size;
			B = iBE ? B - 1 : B + 1) {
			printByte(((char *)ptr) + e * size + B, bitString);
		}
	}

	// print newline character if no string to be returned
	if ( ! retStr ) {
		puts("");
	}

	return bitString;
}

/**************** printByte() ****************/
void printByte(void *ptr, char *dest)
{
	unsigned char *bytePtr = malloc(1);

	// check if bytePtr was allocated properly
	if ( bytePtr == NULL ) {
		fprintf(stderr, "Error: difficulty allocating memory for byte\n");
		return;
	}

	memcpy(bytePtr, ptr, 1);

	for ( int b = 0; b < 8; *bytePtr <<= 1, b++ ) {
		if ( *bytePtr & 128 ) {
			if ( dest == NULL ) {
				printf("1");
			} else {
				strcat(dest, "1");
			}
		} else {
			if ( dest == NULL ) {
				printf("0");
			} else {
				strcat(dest, "0");
			}
		}
	}

	free(bytePtr);
}

/**************** invertBytes ****************/
void invertBytes(void *ptr, const int count)
{
	for ( int B = 0; B < count; B++ ) {
		((char *)ptr)[B] = ~(((char *)ptr)[B]);
	}
}