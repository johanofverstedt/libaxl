
#ifndef LIBAXL_VECTOR_F64_GUARD
#define LIBAXL_VECTOR_F64_GUARD

#include "vectors.h"

namespace libaxl {
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

inline
void linear_add(v64 a, v64 b, f64 scalar_multiplier) {
	auto count = result_length(a, b);
	if(a.stride == 1) {
		if(b.stride == 1) {
			//Use a SIMD-version here
			for(index_type i = 0; i < count; ++i) {
				a.array[i] += b.array[i] * scalar_multiplier;
			}
		} else {
			for(index_type i = 0; i < count; ++i) {
				a.array[i] += b.array[i * b.stride] * scalar_multiplier;
			}
		}
	} else {
		if(b.stride == 1) {
			for(index_type i = 0; i < count; ++i) {
				a.array[i * a.stride] += b.array[i] * scalar_multiplier;
			}
		} else {
			for(index_type i = 0; i < count; ++i) {
				a.array[i * a.stride] += b.array[i * b.stride] * scalar_multiplier;
			}
		}
	}
}

inline
v64 linear_add(v64 a, v64 b, f64 scalar_multiplier, arena* arena) {
	v64 result;
	auto count = result_length(a, b);
	result = make_uninitialized_vector<f64>(arena, count);

	if(a.stride == 1) {
		if(b.stride == 1) {
			//Use a SIMD-version here
			for(index_type i = 0; i < count; ++i) {
				result.array[i] = a.array[i] + b.array[i] * scalar_multiplier;
			}
		} else {
			for(index_type i = 0; i < count; ++i) {
				result.array[i] = a.array[i] + b.array[i * b.stride] * scalar_multiplier;
			}
		}
	} else {
		if(b.stride == 1) {
			for(index_type i = 0; i < count; ++i) {
				result.array[i] = a.array[i * a.stride] + b.array[i] * scalar_multiplier;
			}
		} else {
			for(index_type i = 0; i < count; ++i) {
				result.array[i] = a.array[i * a.stride] + b.array[i * b.stride] * scalar_multiplier;
			}
		}
	}

	return result;
}

inline
v64 linear_blend(v64 a, v64 b, f64 t) {
	v64 result;
	
	auto count = result_length(a, b);//minimum(length(a), length(b));
	result = make_uninitialized_vector<f64>(a.arena, count);

	for(index_type i = 0; i < count; ++i) {
		auto a_value = a[i];
		auto b_value = b[i];

		result.array[i] = a_value + t * (b_value - a_value);
	}

	return result;
}

inline
f64 mean(vector_f64 v) {
	f64 result = 0.0;

	auto len = length(v);
	auto flen = (double)len;

	for(index_type i = 0; i < len; ++i) {
		result += v.array[i];
	}

	result /= flen;

	return result;
}

}

// LIBAXL_VECTOR_F64_GUARD
#endif
