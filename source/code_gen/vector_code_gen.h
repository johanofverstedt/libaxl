
#ifndef LIBAXL_VECTOR_TYPE_CODE_GEN
#define LIBAXL_VECTOR_TYPE_CODE_GEN

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"

#include "include_guards.h"

namespace libaxl {
struct vector_cg_settings {
	const char* project_name;
	const char* type_name;
	const char* sub_type_name;
	const char* out_path;

	//Flags
	bool header_only;
	bool generate_sequential;
	bool generate_reversed;
	bool generate_strided;
	bool generate_stream_operations;
	bool generate_arithmetic_operations;
	bool generate_scalar_expr_eval;
	bool generate_unrolled_expr_eval;
};

}

#endif
