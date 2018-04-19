/* ps4.c
 * Program producing verifiable code for COSC 031 | Problem Set 4
 * usage: ./ps4 p
 *	* P: problem number to run the test for
 *
 * Zeke Baker | 20180419.0008 | ps4.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char *longPalSubseq(char *s);

int main(const int argc, char *argv[])
{
	switch (atoi(argv[1])) {
		// Problem 2: Palindromic Subsequences
		case 2: {
			char *sigma;
			sigma = longPalSubseq("similes");
			printf("LCS of \"similes\" is \"%s\"\n", sigma);
			free(sigma);
			sigma = longPalSubseq("lessee");
			printf("LCS of \"lessee\" is \"%s\"\n", sigma);
			free(sigma);
			sigma = longPalSubseq("Cartagena");
			printf("LCS of \"Cartagena\" is \"%s\"\n", sigma);
			free(sigma);
			sigma = longPalSubseq("ACGTGTCAAAATCG");
			printf("LCS of \"ACGTGTCAAAATCG\" is \"%s\"\n", sigma);
			free(sigma);
		} default:
			exit(1);
	}
}


/* **************** longPalSubseq() ****************
 * Finds the longest palindromic subsequence (LPS) of a given string
 * s: string to find the LPS of
 * return: allocated string of the LPS
 *
 * memory contract: returned string must be freed
 */
char *
longPalSubseq(char *s)
{
	const int n = strlen(s);	// int for size of string s
	int L[n + 1][n + 1];			// 2D int array for LPS(l,m)
	char *sCpy;								// string for copy of s, indexed from 1 to n
	char *sigma;							// string for LPS to be returned

	sCpy = calloc(n + 2, sizeof(char));

	// check if sCpy was allocated properly
	if ( sCpy == NULL ) {
		return NULL;
	}

	strcpy(sCpy + 1, s);
	s = sCpy;

	// populate base cases
	L[1][1] = 1;
	for ( int m = 2; m <= n; m++ ) {
		L[m][m - 1] = 0;
		L[m][m] = 1;
	}

	// populate remaining L such that l < m
	for ( int m = 2; m <= n; m++ ) {
		for ( int l = m - 1; l >= 1; l-- ) {
			// two_lm = s[l] == s[m] ? 2 : 0;
			L[l][m] = L[l + 1][m - 1] + (s[l] == s[m] ? 2 : 0);
			if ( L[l][m] < L[l + 1][m] ) {
				L[l][m] = L[l + 1][m];
			} else if ( L[l][m] < L[l][m - 1] ) {
				L[l][m] = L[l][m - 1];
			}
		}
	}

	// L[1][n] now contains the LCS of s

	// allocate the recovery string
	sigma = calloc(L[1][n] + 1, sizeof(char));

	// check if sigma was allocated properly
	if ( sigma == NULL ) {
		return NULL;
	}

	// invariant: # of occupied chars in sigma + L[l][m] = L[1][n]

	// recover the string
	for ( int l = 1, m = n; l <= m; ) {
		if ( L[l][m] != L[l + 1][m] && L[l][m] != L[l][m - 1] ) {
			// L[l][m] = L[l + 1][m - 1] + 2
			sigma[L[1][n] / 2 - L[l][m] / 2] = s[l];
			sigma[(L[1][n] - 1) / 2 + L[l][m] / 2] = s[m];
			l++;
			m--;
		} else if (  L[l][m] != L[l + 1][m] ) {
			// L[l][m] = L[l][m - 1]
			m--;
		} else {
			// L[l][m] = L[l + 1][m]
			l++;
		}
	}

	return sigma;
}