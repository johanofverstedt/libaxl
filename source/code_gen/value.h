
#ifndef LIBAXL_VALUE_GUARD
#define LIBAXL_VALUE_GUARD

#include <stdint.h>
#include <inttypes.h>

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "type.h"

namespace libaxl {
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

struct value {
	type_header type;
	void* value_ptr;
};

inline
value make_i32_value(arena* arena, i32 x) {
	value result;

	type_header type;
	type.id = type_info_i32;
	type.type_name = "i32";

	result.type = type;

	i32* ptr = allocate<i32>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

inline
value make_u32_value(arena* arena, u32 x) {
	value result;

	type_header type;
	type.id = type_info_u32;
	type.type_name = "u32";

	result.type = type;

	u32* ptr = allocate<u32>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

inline
value make_i64_value(arena* arena, i64 x) {
	value result;

	type_header type;
	type.id = type_info_i64;
	type.type_name = "i64";

	result.type = type;

	i64* ptr = allocate<i64>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

inline
value make_u64_value(arena* arena, u64 x) {
	value result;

	type_header type;
	type.id = type_info_u64;
	type.type_name = "u64";

	result.type = type;

	u64* ptr = allocate<u64>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

inline
value make_float_value(arena* arena, float x) {
	value result;

	type_header type;
	type.id = type_info_float;
	type.type_name = "float";

	result.type = type;

	float* ptr = allocate<float>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

inline
value make_double_value(arena* arena, double x) {
	value result;

	type_header type;
	type.id = type_info_double;
	type.type_name = "double";

	result.type = type;

	double* ptr = allocate<double>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

template <typename T>
inline
T read_value(value v) {
	return *(T*)v.value_ptr;
}

const char* to_string(arena* arena, value x) {
	char* str = allocate<char>(arena, 32);
	switch(x.type.id) {
		case type_info_i32: {
			sprintf(str, PRId32, read_value<i32>(x));
			return str;
		}
		case type_info_u32: {
			sprintf(str, PRIu32, read_value<u32>(x));
			return str;
		}
		case type_info_i64: {
			sprintf(str, PRId64, read_value<i64>(x));
			return str;
		}
		case type_info_u64: {
			sprintf(str, PRIu64, read_value<u64>(x));
			return str;
		}
		case type_info_float: {
			sprintf(str, "%f", read_value<float>(x));
			return str;
		}
		case type_info_double: {
			sprintf(str, "%f", read_value<double>(x));
			return str;
		}
		default:
			break;
	}
	return "[Unknown value]";
}
}

#endif
