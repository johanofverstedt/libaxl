
#ifndef LIBAXL_CIRCULAR_BUFFER_GUARD
#define LIBAXL_CIRCULAR_BUFFER_GUARD

#include "util.h"
#include "arena.h"
#include "vectors.h"
#include "vector_pair.h"

namespace libaxl {

template <typename T>
struct circular_buffer {
	vector<T> buf_vector;
	int tail;
};

template <typename T>
inline
circular_buffer<T> make_circular_buffer(arena* arena, int count) {
	circular_buffer<T> result;

	result.buf_vector = zeros<T>(arena, count);
	result.tail = 0;

	return result;
}

template <typename T>
inline
circular_buffer<T> make_circular_buffer(vector<T> v, int tail = 0) {
	circular_buffer<T> result;

	assert(tail >= 0);
	assert(tail < length(v));

	result.buf_vector = v;
	result.tail = 0;

	return result;
}

template <typename T>
inline
index_type length(circular_buffer<T> c) {
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
template <typename T>
inline
circular_buffer<T> rotate_left(circular_buffer<T> c, int count) {
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
template <typename T>
inline
circular_buffer<T> rotate_right(circular_buffer<T> c, int count) {
	int size = length(c);


	int new_tail = c.tail - count;
	if(new_tail < 0)
		new_tail += size;

	c.tail = new_tail;
	return c;
}

template <typename T>
inline
vector_pair<T> write(circular_buffer<T> c, int count) {
	vector_pair<T> result;

	int size = length(c);
	assert(count <= size);

	result.v[0] = take_at_most(drop(c.buf_vector, c.tail), count);

	int remaining_count = count - length(result.v[0]);
	result.v[1] = take_at_most(take(c.buf_vector, c.tail), remaining_count);

	return result;
}

template <typename T>
inline
vector_pair<T> read(circular_buffer<T> c, int count, int offset) {
	vector_pair<T> result;

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

template <typename T>
inline
vector_pair<T> read(circular_buffer<T> c, int count) {
	vector_pair<T> result;

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
