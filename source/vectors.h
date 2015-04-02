
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

	inline
	vector operator+(vector in);
	inline
	vector operator-(vector in);
	inline
	vector operator*(vector in);
	inline
	vector operator+=(vector in);
	inline
	vector operator-=(vector in);
	inline
	vector operator*=(vector in);
};

//
//  core vector functions
//

inline
bool check(vector v) {
	bool result = (v.array != nullptr) && (stride >= 1) &&
	(width >= 1) && (allocator != nullptr);

	return result;
}

inline
int length(vector v) {
	assert(check(v));

	auto result = (v.end - v.start) / v.stride;

	return result;
}

inline
bool is_empty(vector v) {
	return length(v) == 0;
}

inline
bool is_reverse(vector v) {
	return v.stride < 0;
}

inline
bool is_sequential(vector v) {
	return (v.stride == 1) || (v.stride == -1);
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

	assert(check(in));

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

template <typename BinaryOp>
inline
vector apply_op(vector a, vector b, BinaryOp op) {
	vector result;

	int a_len = length(a);
	int b_len = length(b);

	if(a_len == 0 || b_len == 0) {
		result.array = a.array;
		result.start = 0;
		result.end = 0;
		result.stride = 1;
		result.width = a.width;
		result.allocator = a.allocator;

		return result;
	}

	//
	//  Telescoping
	//

	if(a_len == 1 || b_len == 1) {
		//Todo: fix it
		assert(false);
		return result;
	}

	assert(a.width == b.width);

	int out_len = minimum(a_len, b_len);

	result = make_uninitialized_vector(a.allocator, out_len, a.width);

	for(int i = 0; i < out_len; ++i) {
		for(int j = 0; j < result.width; ++j) {
			result.array[i * result.width + j] =
			  op(a.array[start + i * a.stride + j], b.array[start + i * b.stride + j]);
		}
	}

	return result;
}

//
//  vector operator overloads
//

struct binary_add_op {
	inline
	double operator()(double a, double b) { return a + b; }
};
struct binary_sub_op {
	inline
	double operator()(double a, double b) { return a - b; }
};
struct binary_mul_op {
	inline
	double operator()(double a, double b) { return a * b; }
};

inline
vector vector::operator+(vector in) {
	vector result;

	result = apply_op(*this, in, binary_add_op());
	
	return result;
}

inline
vector vector::operator-(vector in) {
	vector result;

	result = apply_op(*this, in, binary_sub_op());

	return result;
}

inline
vector vector::operator*(vector in) {
	vector result;

	result = apply_op(*this, in, binary_mul_op());
	
	return result;
}

//  Todo: replace with faster versions, that don't allocate new memory

inline
vector vector::operator+=(vector in) {
	(*this) = (*this) + in;
	return *this;
}
inline
vector vector::operator-=(vector in) {
	(*this) = (*this) - in;
	return *this;
}
inline
vector vector::operator*=(vector in) {
	(*this) = (*this) * in;
	return *this;
}
}

#endif
