/* fracToSphere.c
 * Converts a double t in [0, 1] to a Hilbert mapping (x, y) in [0, 1] x [0, 1] 
 * to a point on the unit sphere (x, y, z) where x^2 + y^2 + z^2 = 1
 *
 * usage: ./fracToSphere
 * fracToSphere then requests a number t in [0, 1]. If an EOF or new-line is 
 * entered, the program exits. If multiple tokens are entered, the first is used 
 * and the rest are ignored. If t is not a number, t = 0 is used. If t is 
 * negative, its opposite is used. If t is greater than 1, t = 1 is used.
 *
 * Zeke Baker | 20180316.1123 | Programs/fracToSphere
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "libs/file.h"
#include "libs/hilbert.h"

int main()
{
	double *hilbertPt;					// double ptr for the pt (x, y) by hilbertMap()
	double t = 0;								// double for the time (t) in [0, 1] to map to 2D 
		// then a 3D unit vector
	double theta, phi, x, y, z;	// doubles for the 3D unit vector
	char *line;									// string for line read from stdin
	char *tAsStr;								// string for desired double t
	char *extra;								// string for extra tokens in line

	printf("Enter a number t in [0, 1]: ");
	line = readlinep(stdin);

	// while there's another line to read 
	while ( line != NULL ) {
		tAsStr = strtok(line, " ");
		extra = strtok(NULL, " ");
		
		// check if tAsStr is NULL
		if ( tAsStr == NULL ) {
			free(line);
			break;
		}

		// check if there are any extra tokens in line
		if ( extra != NULL ) {
			puts("Hey now, only one token next time. I'll still find your unit vector though.");
		}

		t = atof(tAsStr);

		// check if t is negative (invalid, but still doable).
		if ( t < 0 ) {
			puts("Token t was less than 0. Finding unit vector for -t instead.");
			t *= -1;
		}

		// check if t is greater than 1 (also invalid, but still doable)
		if ( t > 1 ) {
			puts("Token t was greater than 1. Finding unit vector for t = 1 instead.");
			t = 1;
		}

		hilbertPt = hilbertMap(t);
		theta = 2 * acos(-1) * hilbertPt[0];
		phi = acos(2 * hilbertPt[1] - 1);
		x = cos(theta) * sin(phi);
		y = sin(theta) * sin(phi);
		z = cos(phi);

		printf("Hilbert mapping: (%8f, %8f)\nSpherical coordinate: (1, %8f, %8f)\
			\nCartesian coordinate: (%8f, %8f, %8f)\n", hilbertPt[0], hilbertPt[1], 
			theta, phi, x, y, z);

		free(hilbertPt);
		free(line);
		printf("\nEnter a number t in [0, 1]: ");
		line = readlinep(stdin);
	}
	
	printf("Exiting fracToSphere\n");
	return 0;
}