/* hilbert.h
 * A set of functions to do various things regarding the Hilbert curve
 *
 * Zeke Baker | 20180315.2214 | Programs/libs/hilbert.h
 */

#ifndef __HILBERT_H__
#define __HILBERT_H__

#include <stdio.h>

/**************** functions ****************/

/* **************** hilbertMap() ****************
 * maps a double between 0 and 1 inclusive to its respective coordinate (x, y), 
 * where x and y are in [0, 1], on the Hilbert curve
 *
 * fraction: double of a fraction to find the Hilbert mapping of
 * return: double ptr to two consecutive doubles for the x and y coordinate
 *
 * memory contract: the returned ptr needs to be freed
 * non-assumptions:
 * 	* though fraction should be between 0 and 1 inclusive, sign is ignored 
 *		(negative numbers return the mapping of their positive counterpart), and 
 *		numbers with a magnitude > 1 map to the same point as 1
 */
double *hilbertMap(const double fraction);

#endif // __HILBERT_H__