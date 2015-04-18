
#ifndef LIBAXL_FUNCTION_GUARD
#define LIBAXL_FUNCTION_GUARD

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"
#include "scope.h"

#include "type.h"
#include "variable.h"
#include "statement.h"

namespace libaxl {
	struct function_header {
		int32_t param_count;
		variable* parameters;
		type_header return_type;

		char* name;
	};

	struct function {
		function_header header;

		int32_t statement_count;
		statement* statements;
	};

	inline
	void codegen(cg_context* context, function f) {
		string_buffer& sb = context->sb;
		codegen(context, f.header.return_type);
		append(sb, " ");
		append(sb, f.header.name);
		append(sb, "(");
		for(int i = 0; i < f.header.param_count; ++i) {
			codegen(context, f.header.parameters[i], true);
			if(i != f.header.param_count - 1)
				append(sb, ", ");
		}
		append(sb, ") {");
		indent()

	}

}

#endif
