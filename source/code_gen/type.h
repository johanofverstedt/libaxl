
#ifndef LIBAXL_TYPE_GUARD
#define LIBAXL_TYPE_GUARD

namespace libaxl {
enum type_info_enum {
	type_info_generic,
	type_info_char,
	type_info_bool,
	type_info_i32,
	type_info_u32,
	type_info_i64,
	type_info_u64,
	type_info_float,
	type_info_double,
	type_info_user_defined,
};
struct type_header {
	const char* type_name;
	int id;
};
}

#endif
