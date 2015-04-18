
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
		bool is_public;
	};

	struct function {
		function_header header;

		int32_t statement_count;
		statement* statements;
	};

	inline
	function_header make_function_header(arena* arena, char* name, int32_t param_count) {
		function_header result;

		result.param_count = param_count;
		if(param_count >= 1) {
			result.parameters = allocate<variable>(arena, param_count);
		} else {
			result.parameters = nullptr;
		}

		result.name = name;
		result.is_public = false;

		return result;
	}

	inline
	function make_function(arena* arena, function_header fh, int32_t statement_count) {
		function result;

		result.header = fh;
		result.statement_count = statement_count;
		if(statement_count > 0)
			result.statements = allocate<statement>(arena, statement_count);
		else
			result.statements = nullptr;

		return result;
	}

	inline
	void codegen(cg_context* context, function_header fh) {
		string_buffer& sb = context->sb;
		if(!fh.is_public) {
			append(sb, "static");
			newline(sb, context->indent);
		}
		codegen(context, fh.return_type);
		append(sb, " ");
		append(sb, fh.name);
		append(sb, "(");
		for(int i = 0; i < fh.param_count; ++i) {
			codegen(context, fh.parameters[i], true);
			if(i != fh.param_count - 1)
				append(sb, ", ");
		}
		append(sb, ")");
	}

	inline
	void codegen(cg_context* context, function f) {
		string_buffer& sb = context->sb;
		
		//generate code for function header

		codegen(context, f.header);

		append(sb, " {");
		indent(context);
		newline(sb, context->indent);
		
		//generate code for function body

		for(int32_t i = 0; i < f.statement_count; ++i) {
			codegen(context, f.statements[i]);
			if(i != f.statement_count - 1)
				newline(sb, context->indent);
		}

		unindent(context);
		newline(sb, context->indent);
		append(sb, "} // ");
		append(sb, f.header.name);

		newline(sb, context->indent);
		newline(sb, context->indent);
	}

}

#endif
