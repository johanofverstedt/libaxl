
#ifndef LIBAXL_VECTORS_GUARD
#define LIBAXL_VECTORS_GUARD

#include "util.h"
#include "arena.h"

namespace libaxl {

struct vector {
	double* array;
	arena* arena;
	index_type count;
	index_type stride;
	index_type width;

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
void check(vector v) {
	assert(v.array != nullptr);
	assert(v.stride >= 1 || v.stride <= -1);
	assert(v.width >= 1);
	assert(v.arena != nullptr);
	/*bool result = (v.array != nullptr) && (v.stride >= 1 || v.stride <= -1) &&
	(v.width >= 1) && (v.arena != nullptr);*/
}

inline
index_type length(vector v) {
	return v.count;
}

inline
index_type width(vector v) {
	return v.width;
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

	result.array = v.array + (v.count - 1) * v.stride;
	result.arena = v.arena;
	result.count = v.count;
	result.stride = -v.stride;
	result.width = v.width;

	return result;
}

inline
vector take_at_most(vector v, index_type count) {
	vector result;

	assert(count >= 0);

	if(count > v.count)
		count = v.count;

	result.array = v.array;
	result.arena = v.arena;
	result.count = count;
	result.stride = v.stride;
	result.width = v.width;

	return result;
}

inline
vector take(vector v, index_type count) {
	vector result;

	assert(count >= 0);
	assert(count <= length(v));

	result.array = v.array;
	result.arena = v.arena;
	result.count = count;
	result.stride = v.stride;
	result.width = v.width;

	return result;
}

inline
vector drop_at_most(vector v, index_type count) {
	vector result;

	assert(count >= 0);

	if(count > v.count)
		count = v.count;

	result.array = v.array + count * v.stride;
	result.arena = v.arena;
	result.count = v.count - count;
	result.stride = v.stride;
	result.width = v.width;

	return result;
}

inline
vector drop(vector v, index_type count) {
	vector result;

	assert(count >= 0);
	assert(count <= length(v));

	result.array = v.array + count * v.stride;
	result.count = v.count - count;
	result.stride = v.stride;
	result.width = v.width;
	result.arena = v.arena;

	return result;
}

inline
vector drop_even(vector v) {
	vector result;

	//[1, 2, 3, 4] -> [2, 4]
	//[1, 2, 3, 4, 5] -> [2, 4]

	result.array = v.array + v.stride;
	result.arena = v.arena;
	result.count = v.count / 2;
	result.stride = 2 * v.stride;
	result.width = v.width;

	return result;
}

inline
vector drop_odd(vector v) {
	vector result;

	//[1, 2, 3, 4] -> [1, 3]
	//[1, 2, 3, 4, 5] -> [1, 3, 5]

	result.array = v.array;
	result.arena = v.arena;
	result.count = (v.count + 1) / 2;
	result.stride = 2 * v.stride;
	result.width = v.width;

	return result;
}

//
//  vector factory functions
//

inline
vector make_uninitialized_vector(arena *arena, index_type count, index_type width = 1) {
	vector result;

	assert(arena);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = allocate<double>(arena, len);//arena->alloc(len);
	result.arena = arena;
	result.count = count;
	result.stride = width;
	result.width = width;

	return result;
}

inline
vector zeros(arena *arena, index_type count, index_type width = 1) {
	vector result = make_uninitialized_vector(arena, count, width);
	
	memset(result.array, 0, count * width * sizeof(double));

	return result;
}

inline
vector ones(arena* arena, index_type count, index_type width = 1) {
	vector result = make_uninitialized_vector(arena, count, width);
	
	index_type loop_count = count * width;
	for(index_type i = 0; i < loop_count; ++i)
		result.array[i] = 1.0;

	return result;	
}

inline
vector delta(arena* arena, index_type count, index_type width = 1) {
	vector result = zeros(arena, count, width);
	
	assert(count > 0);

	for(index_type j = 0; j < width; ++j) {
		result.array[j] = 1.0;
	}

	return result;	
}

inline
vector ramp(arena* arena, index_type count, index_type width = 1) {
	vector result = make_uninitialized_vector(arena, count, width);
	
	assert(count > 1);

	double denominator = 1.0 / ((count - 1));
	for(index_type i = 0; i < count; ++i) {
		for(index_type j = 0; j < width; ++j) {
			result.array[i * width + j] = i * denominator;
		}
	}

	return result;
}

inline
vector iota(arena* arena, index_type count, index_type width = 1) {
	vector result = make_uninitialized_vector(arena, count, width);
	
	assert(count > 1);

	for(index_type i = 0; i < count; ++i) {
		for(index_type j = 0; j < width; ++j) {
			result.array[i * width + j] = i;
		}
	}

	return result;	
}

inline
vector wrap_vector(arena* arena, double* array, index_type count, index_type width = 1) {
	vector result;

	assert(arena);
	assert(array);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = array;
	result.arena = arena;
	result.count = count;
	result.stride = width;
	result.width = width;

	return result;
}

inline
vector make_vector(arena* arena, double* array, index_type count, index_type width = 1) {
	vector result;

	assert(arena);
	assert(array);
	assert(count >= 0);
	assert(width >= 1);

	auto len = count * width;

	result.array = allocate<double>(arena, len);//arena->alloc(len);
	result.arena = arena;
	result.count = count;
	result.stride = width;
	result.width = width;

	for(index_type j = 0; j < count; ++j) {
		for(index_type i = 0; i < width; ++i) {
			result.array[j * width + i] = array[i * width + j];
		}
	}

	return result;
}

inline
vector make_vector(arena* arena, double** array, index_type count, index_type width = 1) {
	vector result;

	assert(arena);
	assert(array);
	assert(count >= 0);
	assert(width >= 1);

	auto size = count * width;

	result.array = allocate<double>(arena, size);//arena->alloc(size);
	result.arena = arena;
	result.count = count;
	result.stride = width;
	result.width = width;

	for(index_type j = 0; j < count; ++j) {
		auto result_major_index = j * width;
		for(index_type i = 0; i < width; ++i) {
			result.array[result_major_index + i] = array[i][j];
		}
	}

	return result;
}

//
//  basic content manipulators
//

inline
void fill(vector v, double value) {
	index_type test_count = v.count * v.stride;
	for(index_type i = 0; i < test_count; i += v.stride) {
		v.array[i] = value;
	}
}

inline
void zero(vector v) {
	fill(v, 0.0);
}

inline
double get(vector v, index_type index) {
	assert(index >= 0 && index < length(v));

	return v.array[index * v.stride];
}
inline
double get(vector v, index_type index, index_type sub_index) {
	assert(index >= 0 && index < length(v));
	assert(sub_index >= 0 && sub_index < v.width);

	return v.array[index * v.stride + sub_index];
}
inline
void set(vector v, index_type index, double value) {
	assert(index >= 0 && index < length(v));

	v.array[index * v.stride] = value;
}

inline
void set(vector v, index_type index, index_type sub_index, double value) {
	assert(index >= 0 && index < length(v));
	assert(sub_index >= 0 && sub_index < v.width);

	v.array[index * v.stride + sub_index] = value;
}

inline
double to_scalar(vector v) {
	assert(length(v) > 0);

	return *v.array;
}

inline
double to_scalar(vector v, index_type index) {
	assert(length(v) > 0);
	assert(index >= 0 && index < v.width);

	return v.array[index];
}

inline
vector copy(vector in) {
	vector result;

	check(in);

	auto len = length(in);
	auto size = len * in.width;

	result.array = allocate<double>(in.arena, size);//in.arena->alloc(size);
	result.arena = in.arena;
	result.count = len;
	result.stride = in.width;
	result.width = in.width;

	if(result.width == 1) {
		index_type loop_stride = in.stride;
		index_type in_index = 0;
		index_type result_test_index = len * result.width;

		for(index_type i = 0; i < len; ++i) {
			result.array[i] = in.array[in_index];

			in_index += loop_stride;
		}
	} else {
		index_type loop_stride = in.stride;
		index_type in_major_index = 0;
		index_type result_major_index = 0;
		index_type result_test_index = len * result.width;

		while(result_major_index < result_test_index) {
			for(index_type j = 0; j < result.width; ++j) {
				result.array[result_major_index + j] = in.array[in_major_index + j];
			}

			in_major_index += loop_stride;
			result_major_index += result.width;
		}
	}

	return result;
}

inline
vector copy_to(vector in, vector out) {
	check(in);
	assert(in.width == out.width);

	auto len = minimum(length(in), length(out));
	out.count = len;

	if(out.width == 1) {
		for(index_type i = 0; i < len; ++i) {
			out.array[i * out.stride] = in.array[i * in.stride];
		}
	} else {
		for(index_type i = 0; i < len; ++i) {
			index_type out_major_index = i * out.stride;
			index_type in_major_index = i * in.stride;
			for(index_type j = 0; j < out.width; ++j) {
				out.array[out_major_index + j] = in.array[in_major_index + j];
			}
		}
	}

	return out;
}

template <typename BinaryOp>
inline
vector apply_op(vector a, vector b, BinaryOp op) {
	vector result;

	//Always put the thinner vector first
	if(a.width > b.width) {
		result = apply_op(b, a, op);
		return result;
	}

	index_type a_len = length(a);
	index_type b_len = length(b);

	if(a_len == 0 || b_len == 0) {
		result.array = a.array;
		result.arena = a.arena;
		result.count = 0;
		result.stride = 1;
		result.width = a.width;

		return result;
	}

	//
	//  Telescoping
	//

	if(a_len == 1 || b_len == 1) {
		//Todo: fix it
		assert(false);
		result = zeros(a.arena, length(a), a.width);
		return result;
	}

	assert(a.width == b.width);

	index_type out_len = minimum(a_len, b_len);

	result = make_uninitialized_vector(a.arena, out_len, a.width);

	for(index_type i = 0; i < out_len; ++i) {
		for(index_type j = 0; j < result.width; ++j) {
			result.array[i * result.width + j] =
			  op(a.array[i * a.stride + j], b.array[i * b.stride + j]);
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
vector operator+(vector a, vector b) {
	vector result;

	result = apply_op(a, b, binary_add_op());
	
	return result;
}

inline
vector operator-(vector a, vector b) {
	vector result;

	result = apply_op(a, b, binary_sub_op());

	return result;
}

inline
vector operator*(vector a, vector b) {
	vector result;

	result = apply_op(a, b, binary_mul_op());
	
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
	vector result = zeros(v.arena, 1, v.width);

	auto len = length(v);
	auto flen = (double)len;

	for(index_type i = 0; i < len; ++i) {
		for(index_type j = 0; j < v.width; ++j) {
			result.array[j] += v.array[i * v.width + j];
		}
	}

	for(index_type j = 0; j < v.width; ++j)
		result.array[j] /= flen;

	return result;
}

} // namespace libaxl 

// LIBAXL_VECTORS_GUARD
#endif
