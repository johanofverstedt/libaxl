
#ifndef LIBAXL_CODE_GEN_CONTEXT
#define LIBAXL_CODE_GEN_CONTEXT

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

namespace libaxl {	
struct cg_context {
	arena* arena;
	string_buffer sb;
	int indent;
	int indent_delta;
	const char* project_name;
};

inline
cg_context make_cg_context(arena* arena, const char* project_name, int buffer_size) {
	cg_context result;

	result.arena = arena;
	result.sb = make_string_buffer(arena, buffer_size);
	result.indent = 0;
	result.indent_delta = 4;
	result.project_name = project_name;

	return result;
}
}

#endif
