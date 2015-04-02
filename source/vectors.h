
#ifndef LIBAXL_VECTORS_GUARD
#define LIBAXL_VECTORS_GUARD

#include "util.h"

namespace libaxl {

struct vector {
	double* array;
	int start;
	int end;
	int stride;
	int width;
};

//
//  core vector functions
//

inline
int length(vector v) {
	assert(stride != 0);

	auto result = (v.end - v.start) / v.stride;

	return result;
}

inline
bool is_empty(vector v) {
	return length(v) == 0;
}

inline
vector reverse(vector v) {
	vector result;

	result.array = v.array;
	result.start = v.end - v.stride;
	result.end = v.start - v.stride;
	result.stride = -v.stride;

	return result;
}

inline
vector take(vector v, int count) {
	vector result;

	assert(count >= 0);
	assert(count <= length(v));

	result.array = v.array;
	result.start = v.start;
	result.end = v.start + v.stride * count;
	result.stride = v.stride;

	return result;
}

inline
vector drop(vector v, int count) {
	vector result;

	assert(count >= 0);
	assert(count <= length(v));

	result.array = v.array;
	result.start = v.start + v.stride * count;
	result.end = v.end;
	result.stride = v.stride;

	return result;
}

//
//  basic content manipulators
//

inline
void fill(vector v, double value) {
	for(int i = v.start; i != v.end; i += v.stride) {
		v.array[i] = value;
	}
}

inline
void zero(vector v) {
	fill(v, 0.0);
}

inline
void copy(vector in, vector out, int count) {
	for(int i = 0; i < count; ++i) {
		out.array[out.start + i * out.stride] = in.array[in.start + i * in.stride];
	}
}

inline
void copy(vector in, vector out) {
	int count = minimum(length(in), length(out));

	copy(in, out, count);
}

template <typename T>
inline
vector copy(vector in, T& allocator, int width) {
	int len_in = length(in);
	int count = len_in * width;

	vector result;

	result.array = allocate(allocator, count);
	result.start = 0;
	result.end = len_in;
	result.stride = 1;

	copy(in, result);

	return result;
}

template <typename T>
inline
vector copy(vector in, T& allocator) {
	return copy(in, allocator, 1);
}

}

#endif
