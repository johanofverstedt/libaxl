
#ifndef LIBAXL_SCOPE_CODE_GEN_GUARD
#define LIBAXL_SCOPE_CODE_GEN_GUARD

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"

namespace libaxl {
inline
void indent(cg_context* context) {
	int new_indent = context->indent + context->indent_delta;
	if(new_indent < 0) {
		new_indent = 0;
	}

	context->indent = new_indent;
}

inline
void unindent(cg_context* context) {
	int new_indent = context->indent - context->indent_delta;
	if(new_indent < 0) {
		new_indent = 0;
	}

	context->indent = new_indent;
}

inline
void open_scope(cg_context* context) {
	string_buffer& sb = context->sb;
	
	indent(context);

	append(sb, "{");
	newline(sb, context->indent);
}

inline
void close_scope(cg_context* context) {
	string_buffer& sb = context->sb;

	unindent(context);

	newline(sb, context->indent);
	append(sb, "}");
	newline(sb, context->indent);
}

inline
void close_scope(cg_context* context, const char* comment) {
	string_buffer& sb = context->sb;

	unindent(context);

	newline(sb, context->indent);
	append(sb, "} // ");
	append(sb, comment);
	newline(sb, context->indent);
}

inline
void close_scope_with_semicolon(cg_context* context) {
	string_buffer& sb = context->sb;

	unindent(context);

	newline(sb, context->indent);
	append(sb, "};");
	newline(sb, context->indent);
}

inline
void close_scope_with_semicolon(cg_context* context, const char* comment) {
	string_buffer& sb = context->sb;

	unindent(context);

	newline(sb, context->indent);
	append(sb, "}; // ");
	append(sb, comment);
	newline(sb, context->indent);
}
}

#endif
