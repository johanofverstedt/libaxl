
#ifndef LIBAXL_TYPE_GUARD
#define LIBAXL_TYPE_GUARD

#include "cg_context.h"

namespace libaxl {
enum type_info_enum {
	type_info_error,
	type_info_generic,
	type_info_void,
	type_info_char,
	type_info_bool,
	type_info_i32,
	type_info_u32,
	type_info_i64,
	type_info_u64,
	type_info_float,
	type_info_double,
	type_info_ptr,
	type_info_array,
	type_info_static_array,
	type_info_user_defined,
};
struct type_header {
	const char* type_name;
	int id;

	type_header* sub_type;
};

inline
type_header make_type(const char* name, int id, type_header* sub_type) {
	type_header result;

	result.type_name = name;
	result.id = id;
	result.sub_type = sub_type;

	return result;
}

inline
type_header make_ptr_type(arena* arena, type_header sub_type) {
	type_header result;

	result.type_name = "*";
	result.id = type_info_ptr;
	result.sub_type = allocate<type_header>(arena, 1);
	*result.sub_type = sub_type;

	return result;
}

inline
type_header make_type(const char* name, int id) {
	return make_type(name, id, nullptr);
}

inline
bool equal_type_id(type_header t1, type_header t2) {
	return t1.id == t2.id;
}

inline
void codegen(cg_context* context, type_header th) {
	if(th.sub_type != nullptr) {
		codegen(context, *th.sub_type);
	}
	append(context->sb, th.type_name);
}
inline
void codegen(cg_context* context, type_header th, int type_len, const char* variable_name) {
	auto state = push(context->sb);
	
	if(th.sub_type != nullptr) {
		codegen(context, *th.sub_type);
	}

	if(th.id == type_info_static_array) {
		auto appended_length = push(context->sb) - state;
		if(appended_length <= type_len)
			space(context->sb, 1 + type_len - appended_length);
		//space(context->sb, indent);
		append(context->sb, variable_name);
		append(context->sb, "[]");
	} else if(th.id == type_info_ptr) {
		append(context->sb, "*");
		auto appended_length = push(context->sb) - state;
		if(appended_length <= type_len)
			space(context->sb, 1 + type_len - appended_length);
		append(context->sb, variable_name);		
	} else {
		append(context->sb, th.type_name);
		auto appended_length = push(context->sb) - state;
		if(appended_length <= type_len)
			space(context->sb, 1 + type_len - appended_length);
		append(context->sb, variable_name);		
	}
}

}

#endif
