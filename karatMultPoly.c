/* karatMultPoly.c
 * calculates the sum of the square of polynomials of the digits of pi.txt for
 * COSC 031 | Drill 4
 * usage: ./karatMultPoly
 *
 * Zeke Baker | 20180406.1634 | karatMultPoly.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
// #include "libs/file.h"

typedef struct poly {
	long long int *coeffs;	// long long ptr to int array of polynomial coefficients
	int deg; 						// int for degree of polynomial
} poly_t;

poly_t* naiveMultPoly(poly_t* P, poly_t* Q);
poly_t* karatMultPoly(poly_t* P, poly_t* Q);

int main()
{
	const char E[] = "Error: ";
	const char FILE_NAME[] = "files/pi.txt";
	const int N = 20000;

	FILE *fp;								// FILE ptr to read from
	poly_t *p;							// poly_t ptr for pi polynomial
	poly_t *pS;							// poly_t ptr for pi polynomial squared
	time_t beginTime;				// time_t for the time at process begin
	int diffTime;						// int for the difference between beginTime and endTime
	long long int *coeffs;	// long long int ptr to store numbers in as array
	long long int sOSOPS;		// long long int for sum of squares of pi squared

	fp = fopen(FILE_NAME, "r");

	// check if fp was opened properly
	if ( fp == NULL ) {
		fprintf(stderr, "%scouldn't open file \"%s\"\n", E, FILE_NAME);
		exit(1);
	}

	coeffs = calloc(N + 1, sizeof(long long int));
	beginTime = time(NULL);
	
	// store coefficients in coeffs
	for ( int i = 0; i <= N; i++ ) {
		coeffs[i] = ((int)fgetc(fp)) - 48;
	}

	// // debugging
	// {
	// 	srand(time(NULL));
	// 	printf("expand (");
	// 	for ( int i = 0; i <= N; i++ ) {
	// 		if ( i != 0 ) printf(" %s ", coeffs[i] < 0 ? "" : "+");
	// 		coeffs[i] = (long long int)(((double)rand()) / RAND_MAX * 20 - 10);
	// 		printf("%lld x^%d", coeffs[i], i);
	// 	}
	// 	puts(")^2");
	// }

	fclose(fp);	
	p = calloc(1, sizeof(poly_t));
	p->coeffs = coeffs;
	p->deg = N;
	pS = karatMultPoly(p, p);
	free(coeffs);
	free(p);
	sOSOPS = 0;

	// add the square of each coefficient of pS to sOSOPS
	for ( int i = 0; i <= 2 * N; i++ ) {
		sOSOPS += pS->coeffs[i] * pS->coeffs[i];
	}

	// // debugging
	// {
	// 	puts("");
	// 	for ( int i = 2 * N; i >= 0; i-- ) {
	// 		if ( i != 2 * N ) printf(" %s ", pS->coeffs[i] < 0 ? "" : "+");
	// 		printf("%lld x^%d", pS->coeffs[i], i);
	// 	}
	// 	puts("");
	// }

	free(pS->coeffs);
	free(pS);

	diffTime = (int)difftime(time(NULL), beginTime);
	printf("Done sorting and counting after %02d:%02d:%02d!\n", diffTime / 3600, 
		(diffTime / 60) % 60, diffTime % 60);
	printf("Sum of the squares of the coefficients of the square of the %d-degree polynomial\nof the first %d digits of pi: %lld\n", N, N + 1, sOSOPS);
	exit(0);
}

/* **************** naiveMultPoly() ****************
 * Multiplies two polynomials P and Q by explicitly adding the product of each
 * pair of coefficients
 * P: poly_t ptr of first polynomial to multiply
 * Q: poly_t ptr of second polynomial to multiply
 * return: poly_t ptr of product of P and Q, NULL if error
 *
 * memory contract: the returned pointer (and its coeffs variable) must be freed
 */
poly_t* naiveMultPoly(poly_t* P, poly_t* Q)
{
	poly_t *product;	// poly_t ptr for product of P and Q
	product = calloc(1, sizeof(poly_t));

	// check if product was allocated properly
	if ( product == NULL ) {
		return NULL;
	}

	product->coeffs = calloc(P->deg + Q->deg + 1, sizeof(long long int));

	// check if product->coeffs was allocated properly
	if ( product->coeffs == NULL) {
		free(product);
		return NULL;
	}

	// loop through all coefficients of P
	for ( int p = 0; p <= P->deg; p++ ) {
		// loop through all coefficients of Q
		for ( int q = 0; q <= Q->deg; q++ ) {
			product->coeffs[p + q] += P->coeffs[p] * Q->coeffs[q];
		}
	}

	product->deg = P->deg + Q->deg;

	return product;
}

/* **************** karatMultPoly() ****************
 * Multiplies two polynomials P and Q by performing the Karatsuma algorithm
 * P: poly_t ptr of first polynomial to multiply
 * Q: poly_t ptr of second polynomial to multiply
 * return: poly_t ptr of product of P and Q, NULL if error
 *
 * assumptions:
 *	* P->deg = Q->deg
 *
 * memory contract: the returned pointer (and its coeffs variable) must be freed
 */
poly_t* karatMultPoly(poly_t* P, poly_t* Q)
{
	const int THRESHHOLD = 1;	// int for threshold to switch to naiveMultPoly
	// any nondescript polynomial refers to a polynomial described in the lecture
		// notes
	poly_t *PP;								// poly_t ptr for P' (PPrime) polynomial
	poly_t *P1;								// poly_t ptr for P1 polynomial
	poly_t *P2;								// poly_t ptr for P2 polynomial
	poly_t *QP;								// poly_t ptr for Q' (QPrime) polynomial
	poly_t *Q1;								// poly_t ptr for Q1 polynomial
	poly_t *Q2;								// poly_t ptr for Q2 polynomial
	poly_t *R1;								// poly_t ptr for R1 polynomial
	poly_t *R2;								// poly_t ptr for R2 polynomial
	poly_t *R3;								// poly_t ptr for R3 polynomial
	poly_t *R4;								// poly_t ptr for R4 polynomial
	poly_t *R;								// poly_t ptr for R polynomial (result)
	int n;										// int for degree of P and Q
	int m;										// int for midpoint index

	// check if P->deg <= THRESHHOLD
	if ( P->deg <= THRESHHOLD ) {
		return naiveMultPoly(P, Q);
	}

	n = P->deg;
	m = (n / 2) + (n % 2); 	// equivalent to ceil(n / 2) for discrete values of n

	PP = calloc(1, sizeof(poly_t));
	PP->coeffs = calloc(m + 1, sizeof(long long int));
	PP->deg = m;
	QP = calloc(1, sizeof(poly_t));
	QP->coeffs = calloc(m + 1, sizeof(long long int));
	QP->deg = m;

	// add [pq]1 and [pq]2 as described in the notes
	for ( int i = 0; i < m; i++ ) {
		PP->coeffs[i] = P->coeffs[i] + P->coeffs[m + i];
		QP->coeffs[i] = Q->coeffs[i] + Q->coeffs[m + i];
	}

	// add [PQ][n] if necessary
	if ( n > 2 * m - 1 ) {
		PP->coeffs[m] = P->coeffs[n];
		QP->coeffs[m] = Q->coeffs[n];
	} // else case covered because PP and QP were calloc'ed

	P1 = calloc(1, sizeof(poly_t));
	P1->coeffs = P->coeffs;
	P1->deg = m - 1;
	P2 = calloc(1, sizeof(poly_t));
	P2->coeffs = P->coeffs + m;
	P2->deg = n - m;
	Q1 = calloc(1, sizeof(poly_t));
	Q1->coeffs = Q->coeffs;
	Q1->deg = m - 1;
	Q2 = calloc(1, sizeof(poly_t));
	Q2->coeffs = Q->coeffs + m;
	Q2->deg = n - m;

	R1 = karatMultPoly(P1, Q1);
	R2 = karatMultPoly(P2, Q2);
	R3 = karatMultPoly(PP, QP);

	free(PP->coeffs);
	free(QP->coeffs);
	free(PP);
	free(P1);
	free(P2);
	free(QP);
	free(Q1);
	free(Q2);

	R1->coeffs = realloc(R1->coeffs, (2 * m + 1) * sizeof(long long int));
	R1->coeffs[2 * m - 1] = 0;
	R1->coeffs[2 * m] = 0;
	R2->coeffs = realloc(R2->coeffs, (2 * m + 1) * sizeof(long long int));
	
	// set last two to 0 if necessary
	if ( m > n - m ) {
		R2->coeffs[2 * m - 1] = 0;
		R2->coeffs[2 * m] = 0;
	}

	R4 = calloc(1, sizeof(poly_t));
	R4->coeffs = calloc(2 * m + 1, sizeof(long long int));
	R4->deg = 2 * m;

	// construct R4 array
	for ( int i = 0; i <= 2 * m; i++ ) {
		R4->coeffs[i] = R3->coeffs[i] - R1->coeffs[i] - R2->coeffs[i];
	}

	R = calloc(1, sizeof(poly_t));
	R->coeffs = calloc(2 * n + 1, sizeof(long long int));
	R->deg = 2 * n;

	// construct R array
	for ( int i = 0; i <= 2 * n; i++ ) {
		if ( i <= 2 * m ) {
			R->coeffs[i] += R1->coeffs[i];
		}
		if ( i - m >= 0 && i - m <= 2 * m ) {
			R->coeffs[i] += R4->coeffs[i - m];
		}
		if ( i - 2 * m >= 0 ) {
			R->coeffs[i] += R2->coeffs[i - 2 * m];
		}
	}

	free(R1->coeffs);
	free(R1);
	free(R2->coeffs);
	free(R2);
	free(R3->coeffs);
	free(R3);
	free(R4->coeffs);
	free(R4);

	return R;
}

