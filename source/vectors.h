
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
	bool result = true;
	assert(v.array != nullptr);
	assert(v.stride >= 1 || v.stride <= -1);
	assert(v.width >= 1);
	assert(v.allocator != nullptr);
	/*bool result = (v.array != nullptr) && (v.stride >= 1 || v.stride <= -1) &&
	(v.width >= 1) && (v.allocator != nullptr);*/

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
	result.width = v.width;
	result.allocator = v.allocator;

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
	result.width = v.width;
	result.allocator = v.allocator;

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
	result.width = v.width;
	result.allocator = v.allocator;

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
	
	memset(result.array, 0, count * width * sizeof(double));

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

	double denominator = 1.0 / ((count - 1));
	for(int i = 0; i < count; ++i) {
		for(int j = 0; j < width; ++j) {
			result.array[i * width + j] = i * denominator;
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
			result.array[j * width + i] = array[i * width + j];
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
double get(vector v, int index) {
	assert(index >= 0 && index < length(v));

	return v.array[v.start + index * v.stride];
}
inline
double get(vector v, int index, int sub_index) {
	assert(index >= 0 && index < length(v));
	assert(sub_index >= 0 && sub_index < v.width);

	return v.array[v.start + index * v.stride + sub_index];
}
inline
void set(vector v, int index, double value) {
	assert(index >= 0 && index < length(v));

	v.array[v.start + index * v.stride] = value;
}

inline
void set(vector v, int index, int sub_index, double value) {
	assert(index >= 0 && index < length(v));
	assert(sub_index >= 0 && sub_index < v.width);

	v.array[v.start + index * v.stride + sub_index] = value;
}

inline
double to_scalar(vector v) {
	assert(length(v) > 0);

	return v.array[v.start];
}

inline
double to_scalar(vector v, int index) {
	assert(length(v) > 0);
	assert(index >= 0 && index < v.width);

	return v.array[v.start + index];
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

inline
vector copy_to(vector in, vector out) {
	assert(check(in));
	assert(in.width == out.width);

	auto len = minimum(length(in), length(out));

	if(out.width == 1) {
		for(int i = 0; i < len; ++i) {
			out.array[out.start + i * out.stride] = in.array[in.start + i * in.stride];
		}
	} else {
		for(int i = 0; i < len; ++i) {
			for(int j = 0; j < out.width; ++j) {
				out.array[out.start + i * out.stride + j] = in.array[in.start + i * in.stride + j];
			}
		}
	}

	out.end = out.start + len * out.stride;
	return out;
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
			  op(a.array[a.start + i * a.stride + j], b.array[b.start + i * b.stride + j]);
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

//
//  Mathematical operations
//

inline
vector mean(vector v) {
	vector result = zeros(v.allocator, 1, v.width);

	auto len = length(v);
	auto flen = (double)len;

	for(int i = 0; i < len; ++i) {
		for(int j = 0; j < v.width; ++j) {
			result.array[j] += v.array[i * v.width + j];
		}
	}

	for(int j = 0; j < v.width; ++j)
		result.array[j] /= flen;

	return result;
}

}

#endif
