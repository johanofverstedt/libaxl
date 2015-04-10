
#ifndef LIBAXL_STRUCT_GUARD
#define LIBAXL_STRUCT_GUARD

#include <stdint.h>

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"
#include "scope.h"
#include "type.h"

namespace libaxl {
struct struct_type {
	int32_t member_count;

	const char* name;

	type_header* member_types;
	const char** member_names;
};

inline
struct_type make_struct(arena* arena, const char* name, int32_t member_count) {
	struct_type result;

	result.member_count = member_count;

	result.name = name;

	result.types = allocate<type_header>(arena, member_count);
	result.names = allocate<const char*>(arena, member_count);

	for(int32_t i = 0; i < member_count; ++i) {
		result.member_types[i].id = type_info_generic;
		result.member_types[i].type_name = "__UNSPECIFIED__";

		result.member_names[i] = "__UNSPECIFIED__";
	}

	return result;
}

inline
void set_struct_member_type(struct_type* st, int32_t member_index, type_header th) {
	assert(member_index >= 0);
	assert(member_index < st->member_count);

	st->member_types[member_index] = th;
}

inline
void set_struct_member_name(struct_type* st, int32_t member_index, const char* name) {
	assert(member_index >= 0);
	assert(member_index < st->member_count);

	st->member_names[member_index] = name;
}

inline
void codegen(cg_context* context, struct_type st) {
	string_buffer& sb = context->sb;
	
	newline(sb, context->indent);
	append(sb, "struct ");
	append(sb, st.name);
	append(sb, " ");

	open_scope(context);

	for(int32_t i = 0; i < st.member_count; ++i) {
		append(sb, st.member_types[i].type_name);
		append(sb, " ");
		append(sb, st.member_names[i]);
		append(sb, ";");

		if(i != st.member_count - 1)
			newline(sb, context->indent);
	}

	close_scope(context);
}

}

#endif
