/* squareSummable.c
 * Counts the number of square-summable numbers from 1 to a number for COSC 031 | Drill 6
 * usage: ./squareSummable
 *
 * Zeke Baker | 20180412.2302 | squareSummable.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

bool isSquareSummable(const int B);

int main()
{
	isSquareSummable(4);
	isSquareSummable(8);
	isSquareSummable(73);
	isSquareSummable(99);
	isSquareSummable(200);
	isSquareSummable(1000);
	// srand(time(NULL));
	// isSquareSummable((int)((double)rand() / RAND_MAX * 32767));
}


/* **************** isSquareSummable ****************
 * prints out the number of square-summable numbers from 1 to B, also prints
 * out list of these square-summable numbers and which squares add up to B, if
 * B <= 100 and B is square-summable (otherwise it prints a statment stating
 * that it's not square-summable).
 * B: int to count all the square-summable numbers up to for
 * return: whether or not B is square-summable
 */
bool isSquareSummable(const int B)
{
	const int n = (int)sqrt(B);
	int a[n + 1];					// int array for squares
	int count;						// int for count of square-summable numbers from 1 to B
	int m;								// int to access mth row of F or mth element of S or a
	int b;								// int to access bth column of F
	bool F[n + 1][B + 1];	// 2D bool array for whether or not b can be expressed
		// as a sum of a subset of the first m squares (saved in F[m][b])
	bool iSS[B + 1];			// bool array for whether index is square-summable
	bool S[n + 1];				// bool array for whether a member of a is in the subset
		// of square numbers that sums to B
	bool firstPrint;				// bool for whether or not this is the first print

	// populate a_n and set F[m][0] to true for all m
	for ( m = 0; m <= n; m++ ) {
		a[m] = m * m;
		F[m][0] = true;
	}

	// set F[0][b] to false for all b and ensure all of iSS is false
	for ( b = 0; b <= B; b++ ) {
		iSS[b] = false;
		if ( b > 0 ) {
			F[0][b] = false;
		}
	}

	count = 0;

	// populate F and increment count when necessary
	for ( m = 1; m <= n; m++ ) {
		for ( b = 1; b <= B; b++ ) {
			// check if b - a[m] is negative so as not to index out of range
			if ( b - a[m] < 0 ) {
				F[m][b] = F[m - 1][b];
			} else {
				F[m][b] = F[m - 1][b] || F[m - 1][b - a[m]];
			}

			// if in the final row and current cell is true, increment count
			if ( m == n && F[m][b] ) {
				count++;
				iSS[b] = true;
			}
		}
	}

	printf("Number of square-summable numbers between 1 and %d: %d (diff = %d)\n\n", B, count, B - count);

	// check if a recovery is necessary
	if ( !F[n][B] ) {
		printf("%d isn't square-summable.\n\n", B);
		return false;
	}

	// check if B is too large to be worth recovering S
	if ( B > 100 ) {
		printf("%d is square-summable, but it's too large to be worth recovering.\n\n", B);
		return true;
	}

	firstPrint = true;

	// print out all numbers from 1 to B that are square-summable
	for ( b = 1; b <= B; b++ ) {
		if ( iSS[b] ) {
			if ( firstPrint ) {
				printf("{%d", b);
				firstPrint = false;
			} else {
				printf(", %d", b);
			}
		}
	}

	puts("}\n");

	// ensure all of S is false
	for ( m = 0; m <= n; m++ ) {
		S[m] = false;
	}

	m = n;
	b = B;

	// populate S
	while ( b > 0) {
		if ( b - a[m] >= 0 && F[m - 1][b - a[m]] ) {
			S[m] = true;
			b -= a[m];
		}

		m--;
	}

	firstPrint = true;

	// print out set of numbers whose squares sum to B
	for ( m = 0; m <= n; m++ ) {
		if ( S[m] ) {
			if ( firstPrint ) {
				printf("%d^2", m);
				firstPrint = false;
			} else {
				printf(" + %d^2", m);
			}
		}
	}

	printf(" = %d\n\n", B);
	return true;
}