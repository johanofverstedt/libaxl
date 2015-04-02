
#ifndef LIBAXL_VECTORS_GUARD
#define LIBAXL_VECTORS_GUARD

#include "util.h"
#include "vector_allocator.h"

namespace libaxl {

struct vector {
	double* array;
	int start;
	int end;
	int stride;
	int width;
	vector_allocator* allocator;
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
//  vector factory functions
//

inline
vector make_uninitialized_vector(vector_allocator *allocator, int count, int width = 1) {
	vector result;

	assert(allocator);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = allocator->alloc(len);
	result.start = 0;
	result.end = len;
	result.stride = width;
	result.width = width;
	result.allocator = allocator;

	return result;
}

inline
vector zeros(vector_allocator *allocator, int count, int width = 1) {
	vector result = make_uninitialized_vector(allocator, count, width);
	
	memset(result.array, 0, len * sizeof(double));

	return result;
}

inline
vector ones(vector_allocator* allocator, int count, int width = 1) {
	vector result = make_uninitialized_vector(allocator, count, width);
	
	for(int i = 0; i < result.end; ++i)
		result.array[i] = 1.0;

	return result;	
}

inline
vector delta(vector_allocator* allocator, int count, int width = 1) {
	vector result = make_uninitialized_vector(allocator, count, width);
	
	assert(count > 0);

	for(int j = 0; j < width; ++j) {
		result.array[j] = 1.0;
	}
	for(int i = 1; i < result.end; ++i) {
		for(int j = 0; j < width; ++j) {
			result.array[i * width + j] = 0.0;
		}
	}

	return result;	
}

inline
vector ramp(vector_allocator* allocator, int count, int width = 1) {
	vector result = make_uninitialized_vector(allocator, count, width);
	
	assert(count > 1);

	double denominator = 1.0 / (result.end - 1);
	for(int i = 0; i < result.end; ++i) {
		for(int j = 0; j < width; ++j) {
			result.array[i * width + j] = i / denominator;
		}
	}

	return result;
}

inline
vector wrap_vector(vector_allocator* allocator, double* array, int count, int width = 1) {
	vector result;

	assert(allocator);
	assert(array);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = array;
	result.start = 0;
	result.end = len;
	result.stride = width;
	result.width = width;
	result.allocator = allocator;

	return result;
}

inline
vector make_vector(vector_allocator* allocator, double* array, int count, int width = 1) {
	vector result;

	assert(allocator);
	assert(array);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = allocator->alloc(len);
	result.start = 0;
	result.end = len;
	result.stride = width;
	result.width = width;
	result.allocator = allocator;

	for(int j = 0; j < count; ++j) {
		for(int i = 0; i < width; ++i) {
			result.array[j * width + i] = array[i][j];
		}
	}

	return result;
}

inline
vector make_vector(vector_allocator* allocator, double** array, int count, int width = 1) {
	vector result;

	assert(allocator);
	assert(array);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = allocator->alloc(len);
	result.start = 0;
	result.end = len;
	result.stride = width;
	result.width = width;
	result.allocator = allocator;

	for(int j = 0; j < count; ++j) {
		for(int i = 0; i < width; ++i) {
			result.array[j * width + i] = array[i][j];
		}
	}

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
vector copy(vector in) {
	vector result;

	auto len = length(in);
	auto size = len * in.width;

	result.array = in.allocator->alloc(size);
	result.start = 0;
	result.end = size;
	result.stride = in.width;
	result.width = in.width;
	result.allocator = in.allocator;

	if(result.width == 1) {
		for(int i = 0; i < len; ++i) {
			result.array[i] = in.array[in.start + i * in.stride];
		}
	} else {
		for(int i = 0; i < len; ++i) {
			for(int j = 0; j < result.width; ++j) {
				result.array[i * result.width + j] = in.array[in.start + i * in.stride + j];
			}
		}
	}

	return result;
}
}

#endif
