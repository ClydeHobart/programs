/* longIncSubseq.c
 * Finds the size of the longest increasing subsequence of the first 100,000
 * decimal places of pi.txt for COSC 031 | Drill 7
 * usage: ./longIncSubseq
 *
 * Zeke Baker | 20180417.1729 | longIncSubseq.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "libs/file.h"

char *longIncSubseq(char *A, const int n);
int binarySearchB(char *A, int *B, const int m, const int t0, const int b);

int main()
{
	const int N = 10000;
	const char E[] = "Error: ";
	const char FILE_NAME[] = "files/pi.txt";

	FILE *fpIn;					// FILE ptr to read from
	FILE *fpOut;				// FILE ptr to print to
	char *A;						// string to store numbers in as array
	char *sigma;				// string for the LIS
	char *outFileName;	// string for the name of the output file

	fpIn = fopen(FILE_NAME, "r");

	// check if fpIn was opened properly
	if ( fpIn == NULL ) {
		fprintf(stderr, "%scouldn't open file \"%s\"\n", E, FILE_NAME);
		exit(1);
	}

	A = calloc(N + 1, sizeof(char));

	// check if A was allocated properly
	if ( A == NULL ) {
		fprintf(stderr, "%sA wasn't allocated properly\n", E);
		exit(2);
	}

	// store numbers in A
	for ( int i = 1; i <= N; i++ ) {
		A[i] = (fgetc(fpIn));
	}

	fclose(fpIn);	

	sigma = longIncSubseq((char *)A, N);
	free(A);

	// check if longIncSubseq() returned NULL
	if ( sigma == NULL ) {
		fprintf(stderr, "%slongIncSubseq() returned NULL\n", E);
		exit(3);
	}

	printf("%.13s\n", sigma);
	outFileName = calloc(strlen(FILE_NAME) + 5, 1);

	// check if outFileName was allocated properly
	if ( outFileName == NULL ) {
		fprintf(stderr, "%soutFileName wasn't allocated properly\n", E);
		free(sigma);
		exit(4);
	}

	strcpy(outFileName, FILE_NAME);
	strcat(outFileName, ".LIS");
	fpOut = fopen(outFileName, "w");

	// check if fpOut was opened properly
	if ( fpOut == NULL ) {
		fprintf(stderr, "%sfile \"%s\" wasn't opened properly\n", E, outFileName);
		free(outFileName);
		free(sigma);
		exit(5);
	}

	fprintf(fpOut, "%s", sigma);
	fprintf(stderr, "Successfully saved output to \"%s\"!\n", outFileName);
	fclose(fpOut);
	free(outFileName);
	free(sigma);
	exit(0);	
}


/* **************** longIncSubseq() ****************
 * Finds the size of the longest increasing subsequence ( ) of string A
 * A: string to find the LIS of
 * n: int of size of string A
 * return: string of |LIS| and LIS of A
 */
char *
longIncSubseq(char *A, const int n)
{
	int L[n + 1];		// int array for LIS values such that L[m] is the LIS(A[1:m]) 
		// that ends in A[m]. Here, the array is one extra int long so that the 
		// indices 1 thorugh n can be used
	int B[n + 1];		// int array for the binary searching to find the largest
		// index of L with a given value, also with one extra int for index
		// convenience
	char *retStr;		// string for retuned LIS and recovered subsequence
	int b;					// int for current length of occupied B
	int t;					// int for index returned by binarySearchB()
	int mMax;				// int for the index for the maximum value of L
	int retStrIndex;// int for index of the string to be returned
	int l;					// int for current recovery index;
	int j;					// int for next recovery index;
	bool jFound;		// bool for whether or not j has been found

	L[1] = 1; // base case
	B[1] = 1;	// base case
	b = 1; // B now has one element occupied

	for ( int i = 2; i <= n; i++ ) {
		L[i] = 0;
		B[i] = 0;
	}

	// populate L
	for ( int m = 2; m <= n; m++ ) {
		t = binarySearchB(A, B, m, 1, b);
		L[m] = t + 1;
		B[t + 1] = m;

		// check if b needs to be updated
		if ( t == b ) {
			b++;
		}
	}

	mMax = 1;

	// loop over all other indices of L to find the max
	for ( int m = 2; m <= n; m++ ) {
		// check if mMax needs to be updated
		if ( L[m] > L[mMax] ) {
			mMax = m;
		}
	}

	retStr = calloc(21 + L[mMax], sizeof(char));

	// check if retStr was allocated properly
	if ( retStr == NULL ) {
		return NULL;
	}

	sprintf(retStr, "|LIS| = %-5d\nLIS = ", L[mMax]);

	retStrIndex = 19 + L[mMax]; // one extra char was allocated for NULL byte
	l = mMax;
	retStr[retStrIndex] = A[mMax];
	retStrIndex--;

	// add in the other elements of the subsequence
	while ( L[l] > 1 ) {
		jFound = false;
		j = l - 1;

		// while j hasn't been found, keep checking for j
		while ( !jFound ) {
			// check if j has been found
			if ( L[l] == 1 + L[j] ) {
				jFound = true;
			} else {
				j--;
			}
		}
		retStr[retStrIndex] = A[j];
		retStrIndex--;
		l = j;
	}

	return retStr;
}

/* **************** binarySearchB() ****************
 * Finds the largest t index of B[t0:t0+b-1] such that A[B[t]] <= A[m]
 * A: string to compare the values of using indices stored as values in B
 * B: string to compare the values of by plugging them in as indices to A
 * m: int of index of A of value being compared
 * t0: int of index of first element of B being inpsected
 * b: int of number of elements being inspected within B
 * return: int for largest index t of B such that A[B[t]] <= A[m]
 */
int
binarySearchB(char *A, int *B, const int m, const int t0, const int b)
{
	int mid;	// int for middle index of B[t0:t0+b-1]

	// check if only one element is being inspected
	if ( b == 1 ) {
		return A[B[t0]] <= A[m] ? t0 : t0 - 1;
	}

	mid = (2 * t0 + b - 1) / 2;

	// check if A[B[mid]] > A[m]
	if ( A[B[mid]] > A[m] ) {
		return b == 2 ? t0 - 1 : binarySearchB(A, B, m, t0, mid - t0);
	} else {
		return binarySearchB(A, B, m, mid + 1, t0 + b - 1 - mid);
	}
}

// useful print statments for debugging:

// printf("m: %d, t: %d\nA: \n[%c", m, t, A[1]);
// for ( int i = 2; i <= n; i++ ) {
// 	printf(", %c", A[i]);
// }
// printf("]\nL: \n[%d", L[1]);
// for ( int i = 2; i <= n; i++ ) {
// 	printf(", %d", L[i]);
// }
// printf("]\nB: \n[%d", B[1]);
// for ( int i = 2; i <= n; i++ ) {
// 	printf(", %d", B[i]);
// }
// printf("]\n\n");

// printf("m: %d, t0: %d, b: %d, mid: %d\n", m, t0, b, mid);