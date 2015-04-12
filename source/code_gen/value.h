
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

template <typename T>
inline
value alloc_value(arena* arena, T x) {
	value result;

	T* ptr = allocate<T>(arena, 1);
	*ptr = x;

	result.value_ptr = ptr;

	return result;
}

inline
value make_i32_value(arena* arena, i32 x) {
	value result;

	result = alloc_value<i32>(arena, x);
	type_header type;
	type.id = type_info_i32;
	type.type_name = "i32";

	result.type = type;

	return result;
}

inline
value make_u32_value(arena* arena, u32 x) {
	value result;

	result = alloc_value<u32>(arena, x);

	type_header type;
	type.id = type_info_u32;
	type.type_name = "u32";

	result.type = type;

	return result;
}

inline
value make_i64_value(arena* arena, i64 x) {
	value result;

	result = alloc_value<i64>(arena, x);

	type_header type;
	type.id = type_info_i64;
	type.type_name = "i64";

	result.type = type;

	return result;
}

inline
value make_u64_value(arena* arena, u64 x) {
	value result;

	result = alloc_value<u64>(arena, x);

	type_header type;
	type.id = type_info_u64;
	type.type_name = "u64";

	result.type = type;

	return result;
}

inline
value make_float_value(arena* arena, float x) {
	value result;

	result = alloc_value<float>(arena, x);

	type_header type;
	type.id = type_info_float;
	type.type_name = "float";

	result.type = type;

	return result;
}

inline
value make_double_value(arena* arena, double x) {
	value result;

	result = alloc_value<double>(arena, x);

	type_header type;
	type.id = type_info_double;
	type.type_name = "double";

	result.type = type;

	return result;
}

inline
value make_error_value() {
	value result;

	result.type.id = type_info_error;
	result.type.type_name = "error";
	result.value_ptr = nullptr;

	return result;
}

template <typename T>
inline
T read_value(value v) {
	return *(T*)v.value_ptr;
}

inline
bool equal_type_id(value v1, value v2) {
	return equal_type_id(v1.type, v2.type);
}

const char* to_string(arena* arena, value x) {
	char* str = allocate<char>(arena, 32);
	switch(x.type.id) {
		case type_info_error: {
			sprintf(str, "%s", "error");
			return str;
		}
		case type_info_i32: {
			sprintf(str, "%" PRId32, read_value<i32>(x));
			return str;
		}
		case type_info_u32: {
			sprintf(str, "%" PRIu32, read_value<u32>(x));
			return str;
		}
		case type_info_i64: {
			sprintf(str, "%" PRId64, read_value<i64>(x));
			return str;
		}
		case type_info_u64: {
			sprintf(str, "%" PRIu64, read_value<u64>(x));
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
