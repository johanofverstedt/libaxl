
/**
 *  Hash-function to convert any unsigned int number into
 *  a thread-id for work divison for concurrent graph coloring.
 *
 *  Based on FMV-hash algorithm:
 *  http://www.isthe.com/chongo/tech/comp/fnv/
 *
 *  Note: Assumes a Little Endian architecture.
 *  To implement the algorithm for Big Endian architectures
 *  reverse the loop iteration and offset the hv_ptr.
 *
 *  Author: Johan Öfverstedt
 */

#ifndef UINT_TO_THREAD_ID_H
#define UINT_TO_THREAD_ID_H

#define FMV_OFFSET_32 2166136261U
#define FMV_PRIME_32 16777619U

/**
 *  x            - The value to transform to a thread id.
 *  thread_count - The number of target threads.
 *  return       - A value in the interval [0, thread_count)
 */
inline
unsigned int uint_to_thread_id(unsigned int x, unsigned int thread_count) {
	unsigned int hv = FMV_OFFSET_32;
	unsigned char* x_ptr = (unsigned char*)&x;
	unsigned char* hv_ptr = (unsigned char*)&hv;

	for(unsigned int i = 0U; i < sizeof(unsigned int); ++i) {
		hv *= FMV_PRIME_32;
		(*hv_ptr) ^= x_ptr[i];
	}

	hv ^= (hv >> 16);

	return hv % thread_count;
}

//UINT_TO_THREAD_ID_H
#endif
