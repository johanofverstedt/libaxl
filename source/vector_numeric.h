
/**
 *  General operations supported on all numeric vectors.
 */

#ifndef LIBAXL_VECTOR_NUMERIC_GUARD
#define LIBAXL_VECTOR_NUMERIC_GUARD

#include "vectors.h"

namespace libaxl {
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
}

// LIBAXL_VECTOR_NUMERIC_GUARD
#endif
