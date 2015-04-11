
#ifndef LIBAXL_VARIABLE_GUARD
#define LIBAXL_VARIABLE_GUARD

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "type.h"

namespace libaxl {
struct variable {
	type_header type;
	const char* name;
};

inline
void codegen(cg_context* context, variable x, bool decl, int type_len = -1) {
	string_buffer& sb = context->sb;
	
	if(decl) {
		codegen(context, x.type, type_len, x.name);
		//append(sb, x.type.type_name);
		//append(sb, " ");
	} else {
		append(sb, x.name);
	}
}
}

#endif
