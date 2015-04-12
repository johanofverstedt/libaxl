
#ifndef LIBAXL_STRUCT_GUARD
#define LIBAXL_STRUCT_GUARD

#include <stdint.h>

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"
#include "scope.h"
#include "type.h"

#include "variable.h"

namespace libaxl {
struct struct_type {
	int32_t     member_count;
	const char* name;
	variable*   members;
};

inline
struct_type make_struct(arena* arena, const char* name, int32_t member_count) {
	struct_type result;

	result.member_count = member_count;

	result.name = name;

	result.members = allocate<variable>(arena, member_count);

	for(int32_t i = 0; i < member_count; ++i) {
		result.members[i].type = make_type("__UNSPECIFIED__", type_info_generic);

		result.members[i].name = "__UNSPECIFIED__";
	}

	return result;
}

inline
void set_struct_member_type(struct_type* st, int32_t member_index, type_header th) {
	assert(member_index >= 0);
	assert(member_index < st->member_count);

	st->members[member_index].type = th;
}

inline
void set_struct_member_name(struct_type* st, int32_t member_index, const char* name) {
	assert(member_index >= 0);
	assert(member_index < st->member_count);

	st->members[member_index].name = name;
}

inline
void codegen(cg_context* context, struct_type st) {
	string_buffer& sb = context->sb;
	
	newline(sb, context->indent);
	append(sb, "struct ");
	append(sb, st.name);
	append(sb, " ");

	open_scope(context);

	int32_t max_typename_len = 0;

	for(int32_t i = 0; i < st.member_count; ++i) {
		auto sb_state = push(sb);
		codegen(context, st.members[i].type);
		auto len = pop(sb, sb_state);
		if(len > max_typename_len)
			max_typename_len = len;
	}

	for(int32_t i = 0; i < st.member_count; ++i) {
		codegen(context, st.members[i], true, max_typename_len);
		append(sb, ";");

		if(i != st.member_count - 1)
			newline(sb, context->indent);
	}

	close_scope_with_semicolon(context, st.name);
}

}

#endif
