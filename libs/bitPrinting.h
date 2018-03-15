#ifndef __BIT_PRINTING_H__
#define __BIT_PRINTING_H__

#include <stdio.h>

/**************** functions ****************/

/* **************** isBigEndian() ****************
 * returns whether or not the host is big-endian
 *
 * return: host is big-endian ? 1 : 0
 */
char isBigEndian();

/* **************** printBEBits() ****************
 * print bits in big-endian order
 *
 * ptr: void ptr of bits to print
 * size: const int of size of data type in bytes
 * count: const int of count of data type elements to print
 * retStr: const char indicating if a string of bits should be returned
 * return: retStr != NULL ? char ptr of B-E bits : NULL
 * 
 * memory contract: if retStr isn't NULL, the returned string must be freed
 */
char *printBEBits(void *ptr, const int size, const int count, const char retStr);

/* **************** printLEBits() ****************
 * print bits in little-endian order
 *
 * ptr: void ptr of bits to print
 * size: const int of size of data type in bytes
 * count: const int of count of data type elements to print
 * retStr: const char indicating if a string of bits should be returned
 * return: retStr != NULL ? char ptr of L-E bits : NULL
 * 
 * memory contract: if retStr isn't NULL, the returned string must be freed
 */
char *printLEBits(void *ptr, const int size, const int count, const char retStr);

/* **************** printByte() ****************
 * print a byte
 *
 * ptr: void ptr of bits to print
 * dest: string ptr of where to print to (NULL if stdout is destination)
 */
void printByte(void *ptr, char *dest);

/* **************** invertBytes() ****************
 * inverts bytes
 *
 * ptr: void ptr of bytes to invert
 * count: const int of count of bytes to invert
 */
void invertBytes(void *ptr, const int count);

#endif // __BIT_PRINTING_H__