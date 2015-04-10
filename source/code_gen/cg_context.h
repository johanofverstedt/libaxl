
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
	const char* project_name;
};
}

#endif
