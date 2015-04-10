
#ifndef LIBAXL_SCOPE_CODE_GEN_GUARD
#define LIBAXL_SCOPE_CODE_GEN_GUARD

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"

namespace libaxl {
inline
void open_scope(cg_context* context) {
	string_buffer& sb = context->sb;
	
	context->indent += 4;

	append(sb, "{");
	newline(sb);
}

inline
void close_scope(cg_context* context) {
	string_buffer& sb = context->sb;

	int new_indent = context->indent - 4;
	if(new_indent < 0)
		new_indent = 0;

	context->indent = new_indent;
	newline(sb, context->indent);
	append(sb, "}");
	newline(sb, context->indent);
}
}

#endif
