
#ifndef LIBAXL_CIRCULAR_BUFFER_GUARD
#define LIBAXL_CIRCULAR_BUFFER_GUARD

#include "util.h"
#include "vector_allocator.h"
#include "vectors.h"
#include "vector_pair.h"

namespace libaxl {

struct circular_buffer {
	vector buf_vector;
	int tail;
};

inline
circular_buffer make_circular_buffer(vector_allocator* allocator, int count, int width) {
	circular_buffer result;

	result.buf_vector = zeros(allocator, count, width);
	result.tail = 0;

	return result;
}

inline
circular_buffer make_circular_buffer(vector v, int tail = 0) {
	circular_buffer result;

	assert(tail >= 0);
	assert(tail < length(v));

	result.buf_vector = v;
	result.tail = 0;

	return result;
}

inline
int length(circular_buffer c) {
	return length(c.buf_vector);
}

/**
 *  Rotates the circular buffer to the left.
 *
 *  Abstractly:
 *  rotate_left ([0, 1, 2, 3, 4], 2) yields the transformation
 *  [0, 1, 2, 3, 4] -> [2, 3, 4, 0, 1]
 *
 *  Preconditions:
 *  (1) c is a valid circular_buffer
 *  (1) count >= 0
 *  (2) count < length(c)
 *
 *  Returns a new circular buffer which is rotated.
 *  No copy of the contents is made, and no data is actually
 *  moved in memory.
 */
inline
circular_buffer rotate_left(circular_buffer c, int count) {
	int size = length(c);

	int new_tail = c.tail + count;
	if(new_tail >= size)
		new_tail -= size;

	c.tail = new_tail;
	return c;
}

/**
 *  Rotates the circular buffer to the right.
 *
 *  Abstractly:
 *  rotate_right ([0, 1, 2, 3, 4], 2) yields the transformation
 *  [0, 1, 2, 3, 4] -> [3, 4, 0, 1, 2]
 *
 *  Preconditions:
 *  (1) c is a valid circular_buffer
 *  (1) count >= 0
 *  (2) count < length(c)
 *
 *  Returns a new circular buffer which is rotated.
 *  No copy of the contents is made, and no data is actually
 *  moved in memory.
 */
inline
circular_buffer rotate_right(circular_buffer c, int count) {
	int size = length(c);


	int new_tail = c.tail - count;
	if(new_tail < 0)
		new_tail += size;

	c.tail = new_tail;
	return c;
}

inline
vector_pair write(circular_buffer c, int count) {
	vector_pair result;

	int size = length(c);
	assert(count <= size);

	result.v[0] = take_at_most(drop(c.buf_vector, c.tail), count);

	int remaining_count = count - length(result.v[0]);
	result.v[1] = take_at_most(take(c.buf_vector, c.tail), remaining_count);

	return result;
}

inline
vector_pair read(circular_buffer c, int count, int offset) {
	vector_pair result;

	int size = length(c);
	assert(offset >= 0);
	assert(count + offset <= size);

	int read_tail = c.tail - (count + offset);
	if(read_tail < 0)
		read_tail += size;

	result.v[0] = take_at_most(drop(c.buf_vector, read_tail), count);

	int remaining_count = count - length(result.v[0]);
	result.v[1] = take_at_most(take(c.buf_vector, read_tail), remaining_count);

	return result;
}

inline
vector_pair read(circular_buffer c, int count) {
	vector_pair result;

	int size = length(c);
	assert(count <= size);

	int read_tail = c.tail - count;
	if(read_tail < 0)
		read_tail += size;

	result.v[0] = take_at_most(drop(c.buf_vector, read_tail), count);

	int remaining_count = count - length(result.v[0]);
	result.v[1] = take_at_most(take(c.buf_vector, read_tail), remaining_count);

	return result;
}

}

// LIBAXL_CIRCULAR_BUFFER_GUARD
#endif
