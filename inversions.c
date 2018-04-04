/* inversions.c
 * Counts the inversions of pi.txt for COSC 031 | Drill 3
 * usage: ./inversions
 *
 * Zeke Baker | 20180403.2357 | inversions.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "libs/file.h"

typedef struct arrayCount {
	char *arr;						// ptr to array as string
	int arrSize; 					// int for size of string arr
	long long int count;	// long long int for count associated with arr
} arrayCount_t;

arrayCount_t* sortAndCount(char *A, const int n);
arrayCount_t* combineAndCountCrosses(char *P, const int p, char *Q, const int q);

int main()
{
	const char E[] = "Error: ";
	const char FILE_NAME[] = "files/pi.txt";

	FILE *fpIn;					// FILE ptr to read from
	FILE *fpOut;				// FILE ptr to print to
	arrayCount_t *aC;		// arrayCount_t ptr to store sorted string, its size, and
		// inversion count to
	time_t beginTime;		// time_t for the time at process begin
	time_t endTime;			// time_t for the time at process end
	int diffTime;				// int for the difference between beginTime and endTime
	char *numArray;			// string ptr to store numbers in as array
	char *outFileName;	// string ptr for the name of the output file

	fpIn = fopen(FILE_NAME, "r");

	// check if fpIn was opened properly
	if ( fpIn == NULL ) {
		fprintf(stderr, "%scouldn't open file \"%s\"\n", E, FILE_NAME);
		exit(1);
	}


	beginTime = time(NULL);
	numArray = readfilep(fpIn);
	fclose(fpIn);	

	// check if numArray was read properly
	if ( numArray == NULL ) {
		fprintf(stderr, "%sfile \"%s\" wasn't read properly\n", E, FILE_NAME);
		exit(2);
	}

	aC = sortAndCount(numArray, (int)strlen(numArray));
	endTime = time(NULL);
	diffTime = (int)difftime(endTime, beginTime);

	free(numArray);
	printf("Done sorting and counting after %02d:%02d:%02d!\n", diffTime / 3600, 
		(diffTime / 60) % 60, diffTime % 60);

	// check if sortAndCount returned NULL
	if ( aC == NULL ) {
		fprintf(stderr, "%ssortAndCount() returned NULL\n", E);
		exit(3);
	}

	printf("Array size: %d\nNumber of inversions: %lld\n", aC->arrSize, aC->count);
	outFileName = calloc(strlen(FILE_NAME) + 5, 1);

	// check if outFileName was allocated properly
	if ( outFileName == NULL ) {
		fprintf(stderr, "%soutFileName wasn't allocated properly\n", E);
		free(aC->arr);
		free(aC);
		exit(4);
	}

	strcpy(outFileName, FILE_NAME);
	strcat(outFileName, ".out");
	fpOut = fopen(outFileName, "w");

	// check if fpOut was opened properly
	if ( fpOut == NULL ) {
		fprintf(stderr, "%sfile \"%s\" wasn't opened properly\n", E, outFileName);
		free(outFileName);
		free(aC->arr);
		free(aC);
		exit(5);
	}

	fprintf(fpOut, "Array size: %d\nNumber of inversions: %lld\n%s", aC->arrSize, 
		aC->count, aC->arr);
	fprintf(stderr, "Successfully saved output to \"%s\"!\n", outFileName);
	fclose(fpOut);
	free(outFileName);
	free(aC->arr);
	free(aC);
	exit(0);	
}


/* **************** sortAndCount() ****************
 * sorts an array and counts its inversions in O(n*log(n)) time
 * A:	string of array to sort
 * n: int of size of A (A is accessible through index n - 1)
 * return: arrayCount_t ptr containing a ptr to the sorted array and an int of the
 * 		inversion count
 */
arrayCount_t*
sortAndCount(char *A, const int n)
{
	arrayCount_t *result;	// arrayCount_t ptr for sorted A and count of inversions
	arrayCount_t *subAC1;	// arrayCount_t ptr for first sorted sub array and count
		// of inversions
	arrayCount_t *subAC2;	// arrayCount_t ptr for second sorted sub array and
		// count of inversions
	int m;								// int for the middle index;

	// base case: n == 1
	if ( n == 1 ) {
		result = calloc(1, sizeof(arrayCount_t));
		result->arr = calloc(2, 1);
		result->arr[0] = A[0];
		result->arrSize = 1;
		result->count = 0;
		return result;
	}

	m = n / 2;
	subAC1 = sortAndCount(A, m);
	subAC2 = sortAndCount(A + m, n - m);
	result = combineAndCountCrosses(subAC1->arr, subAC1->arrSize, subAC2->arr, 
		subAC2->arrSize);
	result->count += subAC1->count + subAC2->count;
	free(subAC1);
	free(subAC2);
	return result;
}

/* **************** combineAndCountCrosses() ****************
 * Combines two arrays into a sorted array, counting cross inversions as it
 * goes. All this is done in O(n) time, where n is p + q
 * P:	string of one sorted array to combine with
 * p: int of size of P (P is accessible through index p - 1)
 * Q: string of other sorted array to combine with
 * q: int of size of Q (Q is accessible through index q - 1)
 * return: arrayCount_t ptr containing a ptr to the combined array and an int of
 * 		the cross inversion count
 */
arrayCount_t*
combineAndCountCrosses(char *P, const int p, char *Q, const int q)
{
	arrayCount_t *aC; // arrayCount_t ptr for the combined array and count of
		// cross inversions
	int i = 0;				// int for the current index of P under inspection
	int j = 0;				// int for the current index of Q under inspection
	int k = 0;				// int for the current index of aC->arr to write
	
	aC = calloc(1, sizeof(arrayCount_t));
	
	// check if aC was allocated properly
	if ( aC == NULL ) {
		return NULL;
	}

	aC->arr = calloc(1, p + q + 1); // one extra byte for the terminating null byte

	// check if aC->arr was allocated properly
	if ( aC->arr == NULL ) {
		free(aC);
		return NULL;
	}

	aC->arrSize = p + q;
	aC->count = 0;

	while ( i < p && j < q ) {
		if ( P[i] > Q[j] ) {
			aC->arr[k] = Q[j];
			aC->count += p - i;
			j++;
		} else {
			aC->arr[k] = P[i];
			i++;
		}
		k++;
	}

	// concatenate what hasn't been evaluated to aC->arr
	if ( i == p ) {
		strcat(aC->arr, Q + j);
	} else {
		strcat(aC->arr, P + i);
	}

	// because P and Q are sorted, they are allocated arrays
	free(P);
	free(Q);

	return aC;
}