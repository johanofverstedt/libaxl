
#ifndef LIBAXL_VECTORS_GUARD
#define LIBAXL_VECTORS_GUARD

#include "util.h"
#include "arena.h"

namespace libaxl {

template <typename T>
struct vector {
	T* array;
	arena* arena;
	index_type count;
	index_type stride;

	inline T& operator[](index_type index);
};

//
//  standard type definitions
//

using f32 = float;
using f64 = double;

using vector_f32 = vector<float>;
using vector_f64 = vector<double>;
using vector_s8 = vector<int8_t>;
using vector_u8 = vector<uint8_t>;
using vector_s16 = vector<int16_t>;
using vector_u16 = vector<uint16_t>;
using vector_s32 = vector<int32_t>;
using vector_u32 = vector<uint32_t>;
using vector_s64 = vector<int64_t>;
using vector_u64 = vector<uint64_t>;

using v32 = vector<f32>;
using v64 = vector<f64>;

using string = vector<char>;

//
//  core vector functions
//

template <typename T>
inline
bool check(vector<T> v) {
	assert(v.array != nullptr);
	assert(v.stride >= 1 || v.stride <= -1);
	assert(v.arena != nullptr);
	/*bool result = (v.array != nullptr) && (v.stride >= 1 || v.stride <= -1) &&
	(v.width >= 1) && (v.arena != nullptr);*/

	return true;
}

template <typename T>
inline
index_type length(vector<T> v) {
	return v.count;
}

template <typename T1, typename T2>
inline
index_type result_length(vector<T1> v1, vector<T2> v2) {
	return minimum(length(v1), length(v2));
}
template <typename T1, typename T2, typename T3>
inline
index_type result_length(vector<T1> v1, vector<T2> v2, vector<T3> v3) {
	return minimum(minimum(length(v1), length(v2)), length(v3));
}

template <typename T>
inline
size_type value_type_size(vector<T> v) {
	return (size_type) sizeof(v.array[0]);
}

template <typename T>
inline
bool is_empty(vector<T> v) {
	return length(v) == 0;
}

template <typename T>
inline
bool is_reverse(vector<T> v) {
	return v.stride < 0;
}

template <typename T>
inline
bool is_sequential(vector<T> v) {
	return (v.stride == 1) || (v.stride == -1);
}

template <typename T>
inline
vector<T> reverse(vector<T> v) {
	vector<T> result;

	result.array = v.array + (v.count - 1) * v.stride;
	result.arena = v.arena;
	result.count = v.count;
	result.stride = -v.stride;

	return result;
}

template <typename T>
inline
vector<T> take_at_most(vector<T> v, index_type count) {
	vector<T> result;

	assert(count >= 0);

	if(count > v.count)
		count = v.count;

	result.array = v.array;
	result.arena = v.arena;
	result.count = count;
	result.stride = v.stride;

	return result;
}

template <typename T>
inline
vector<T> take(vector<T> v, index_type count) {
	vector<T> result;

	assert(count >= 0);
	assert(count <= length(v));

	result.array = v.array;
	result.arena = v.arena;
	result.count = count;
	result.stride = v.stride;

	return result;
}

template <typename T>
inline
vector<T> drop_at_most(vector<T> v, index_type count) {
	vector<T> result;

	assert(count >= 0);

	if(count > v.count)
		count = v.count;

	result.array = v.array + count * v.stride;
	result.arena = v.arena;
	result.count = v.count - count;
	result.stride = v.stride;

	return result;
}

template <typename T>
inline
vector<T> drop(vector<T> v, index_type count) {
	vector<T> result;

	assert(count >= 0);
	assert(count <= length(v));

	result.array = v.array + count * v.stride;
	result.count = v.count - count;
	result.stride = v.stride;
	result.arena = v.arena;

	return result;
}

template <typename T>
inline
vector<T> drop_even(vector<T> v) {
	vector<T> result;

	//[1, 2, 3, 4] -> [2, 4]
	//[1, 2, 3, 4, 5] -> [2, 4]

	result.array = v.array + v.stride;
	result.arena = v.arena;
	result.count = v.count / 2;
	result.stride = 2 * v.stride;

	return result;
}

template <typename T>
inline
vector<T> drop_odd(vector<T> v) {
	vector<T> result;

	//[1, 2, 3, 4] -> [1, 3]
	//[1, 2, 3, 4, 5] -> [1, 3, 5]

	result.array = v.array;
	result.arena = v.arena;
	result.count = (v.count + 1) / 2;
	result.stride = 2 * v.stride;

	return result;
}

template <typename T, typename Op>
inline
void for_each(vector<T> v, Op op) {
	double* end = v.array + (v.count * v.stride);
	for(double* ptr = v.array; ptr != end; ptr += v.stride) {
		op(*ptr);
	}
}

//
//  vector factory functions
//

template <typename T>
inline
vector<T> make_uninitialized_vector(arena *arena, index_type count) {
	vector<T> result;

	assert(arena);
	assert(count >= 0);

	result.array = allocate<T>(arena, count);
	result.arena = arena;
	result.count = count;
	result.stride = 1;

	return result;
}

template <typename T>
inline
vector<T> zeros(arena *arena, index_type count) {
	auto result = make_uninitialized_vector<T>(arena, count);
	
	memset(result.array, 0, count * sizeof(T));

	return result;
}

//
//  Todo: Pull these out into a separate file!!!
//

inline
vector_f64 ones_f64(arena* arena, index_type count) {
	vector_f64 result = make_uninitialized_vector<f64>(arena, count);
	
	for(index_type i = 0; i < count; ++i)
		result.array[i] = 1.0;

	return result;
}

inline
vector_f64 delta_f64(arena* arena, index_type count) {
	vector_f64 result = zeros<f64>(arena, count);
	
	assert(count > 0);

	result.array[0] = 1.0;

	return result;
}

inline
vector_f64 ramp_f64(arena* arena, index_type count) {
	auto result = make_uninitialized_vector<f64>(arena, count);
	
	assert(count > 1);

	double denominator = 1.0 / ((count - 1));
	for(index_type i = 0; i < count; ++i)
		result.array[i] = i * denominator;

	return result;
}

inline
vector_f64 iota_f64(arena* arena, index_type count) {
	auto result = make_uninitialized_vector<f64>(arena, count);
	
	assert(count > 1);

	for(index_type i = 0; i < count; ++i) {
		result.array[i] = i;
	}

	return result;	
}

template <typename T>
inline
vector<T> wrap_vector(arena* arena, T* array, index_type count) {
	vector<T> result;

	assert(arena);
	assert(array);
	assert(count >= 0);

	result.array = array;
	result.arena = arena;
	result.count = count;
	result.stride = 1;

	return result;
}

template <typename T>
inline
vector<T> make_vector(arena* arena, T* array, index_type count) {
	vector<T> result;

	assert(arena);
	assert(array);
	assert(count >= 0);

	result.array = allocate<T>(arena, count);
	result.arena = arena;
	result.count = count;
	result.stride = 1;

	for(index_type i = 0; i < count; ++i) {
		result.array[i] = array[i];
	}

	return result;
}

//
//  basic content manipulators
//

template <typename T, typename V>
inline
void fill(vector<T> v, V value) {
	for_each(v, [value](T& elem) -> void { elem = value; });
}

inline
void zero(vector_f64 v) {
	fill(v, 0.0);
}

template <typename T>
inline
T get(vector<T> v, index_type index) {
	assert(index >= 0 && index < length(v));

	return v.array[index * v.stride];
}

template <typename T, typename V>
inline
void set(vector<T> v, index_type index, V value) {
	assert(index >= 0 && index < length(v));

	v.array[index * v.stride] = value;
}

template <typename T>
inline
vector<T> copy(vector<T> in) {
	vector<T> result;

	assert(check(in));

	auto len = length(in);

	result.array = allocate<double>(in.arena, len);//in.arena->alloc(size);
	result.arena = in.arena;
	result.count = len;
	result.stride = in.width;

	index_type loop_stride = in.stride;
	index_type in_index = 0;
	index_type result_test_index = len * result.width;

	for(index_type i = 0; i < len; ++i) {
		result.array[i] = in.array[in_index];

		in_index += loop_stride;
	}

	return result;
}

template <typename T>
inline
vector<T> copy_to(vector<T> in, vector<T> out) {
	assert(check(in));

	auto len = minimum(length(in), length(out));
	out.count = len;

	for(index_type i = 0; i < len; ++i) {
		out.array[i * out.stride] = in.array[i * in.stride];
	}

	return out;
}

template <typename T1, typename T2, typename BinaryOp>
inline
vector<decltype(op(*a.array, *b.array))>
apply_op(vector<T1> a, vector<T2> b, BinaryOp op) {
	vector<decltype(op(*a.array, *b.array))> result;

	index_type a_len = length(a);
	index_type b_len = length(b);

	if(a_len == 0 || b_len == 0) {
		result.array = a.array;
		result.arena = a.arena;
		result.count = 0;
		result.stride = 1;

		return result;
	}

	//
	//  Telescoping
	//

	if(a_len == 1 || b_len == 1) {
		//Todo: fix it
		assert(false);
		result = make_uninitialized_vector(a.arena, length(a)); //zeros(a.arena, length(a));
		return result;
	}

	assert(a.width == b.width);

	index_type out_len = minimum(a_len, b_len);

	result = make_uninitialized_vector(a.arena, out_len);

	for(index_type i = 0; i < out_len; ++i) {
		result.array[i] =
			op(a.array[i * a.stride], b.array[i * b.stride]);
	}

	return result;
}

//
//  vector operator overloads
//

template <typename T1, typename T2>
inline
auto operator+(vector<T1> a, vector<T2> b) -> vector<decltype(a.array[0] + b.array[0])> {
	vector<decltype(a.array[0] + b.array[0])> result = make_uninitialized_vector<f64>(a.arena, minimum(a.count, b.count)); //apply_op(a, b, binary_add_op<double, double>());
	
	for (index_type i = 0; i < result.count; ++i)
		result.array[i] = a.array[i * a.stride] + b.array[i * b.stride];

	return result;
}
template <typename T1, typename T2>
inline
auto operator-(vector<T1> a, vector<T2> b) -> vector<decltype(a.array[0] - b.array[0])> {
	vector<decltype(a.array[0] - b.array[0])> result = make_uninitialized_vector<f64>(a.arena, minimum(a.count, b.count)); //apply_op(a, b, binary_add_op<double, double>());

	for (index_type i = 0; i < result.count; ++i)
		result.array[i] = a.array[i * a.stride] - b.array[i * b.stride];

	return result;
}
template <typename T1, typename T2>
inline
auto operator*(vector<T1> a, vector<T2> b) -> vector<decltype(a.array[0] * b.array[0])> {
	vector<decltype(a.array[0] * b.array[0])> result = make_uninitialized_vector<f64>(a.arena, minimum(a.count, b.count)); //apply_op(a, b, binary_add_op<double, double>());

	for (index_type i = 0; i < result.count; ++i)
		result.array[i] = a.array[i * a.stride] * b.array[i * b.stride];

	return result;
}
template <typename T1, typename T2>
inline
auto operator/(vector<T1> a, vector<T2> b) -> vector<decltype(a.array[0] / b.array[0])> {
	vector<decltype(a.array[0] / b.array[0])> result = make_uninitialized_vector<f64>(a.arena, minimum(a.count, b.count)); //apply_op(a, b, binary_add_op<double, double>());

	for (index_type i = 0; i < result.count; ++i)
		result.array[i] = a.array[i * a.stride] / b.array[i * b.stride];

	return result;
}

template <typename T1, typename T2>
inline
vector<T1>& operator+=(vector<T1> &a, vector<T2> b) {
	auto a_array = a.array;
	auto a_count = a.count;
	auto a_stride = a.stride;

	auto count = minimum(a_count, b.count);

	for(index_type i = 0; i < count; ++i) {
		a_array[i * a_stride] += b.array[i * b.stride];
	}

	return a;
}
template <typename T1, typename T2>
inline
vector<T1>& operator-=(vector<T1> &a, vector<T2> b) {
	auto a_array = a.array;
	auto a_count = a.count;
	auto a_stride = a.stride;

	auto count = minimum(a_count, b.count);

	for(index_type i = 0; i < count; ++i) {
		a_array[i * a_stride] -= b.array[i * b.stride];
	}

	return a;
}
template <typename T1, typename T2>
inline
vector<T1>& operator*=(vector<T1> &a, vector<T2> b) {
	auto a_array = a.array;
	auto a_count = a.count;
	auto a_stride = a.stride;

	auto count = minimum(a_count, b.count);

	for(index_type i = 0; i < count; ++i) {
		a_array[i * a_stride] *= b.array[i * b.stride];
	}

	return a;
}
template <typename T1, typename T2>
inline
vector<T1>& operator/=(vector<T1> &a, vector<T2> b) {
	auto a_array = a.array;
	auto a_count = a.count;
	auto a_stride = a.stride;

	auto count = minimum(a_count, b.count);

	for(index_type i = 0; i < count; ++i) {
		a_array[i * a_stride] /= b.array[i * b.stride];
	}

	return a;
}

template <typename T>
inline
T& vector<T>::operator[](index_type index) {
	assert(index >= 0);
	assert(index < count);

	return array[index * stride];
}

} // namespace libaxl

// LIBAXL_VECTORS_GUARD
#endif
