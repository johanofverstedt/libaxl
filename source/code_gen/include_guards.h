
#ifndef LIBAXL_INCLUDE_GUARDS_CODE_GEN_GUARD
#define LIBAXL_INCLUDE_GUARDS_CODE_GEN_GUARD

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "cg_context.h"

namespace libaxl {
inline
char* ucase(arena* arena, const char* input_string) {
	index_type len = strlen(input_string);
	char* str = allocate<char>(arena, len + 1);
	for(index_type i = 0; i <= len; ++i) {
		if(input_string[i] >= 'a' && input_string[i] <= 'z') {
			str[i] = input_string[i] - 32;
		} else {
			str[i] = input_string[i];
		}
	}

	return str;
}

inline
void write_include_guard(cg_context* context, const char* file_name, bool open) {
	string_buffer& sb = context->sb;
	const char* project_name_upper = ucase(context->arena, context->project_name);
	const char* file_name_upper = ucase(context->arena, file_name);

	if(open) {
		append(sb, "#ifndef ");
		append(sb, project_name_upper);
		append(sb, "_");
		append(sb, file_name_upper);
		append(sb, "_GUARD");
		newline(sb);
		append(sb, "#define ");
		append(sb, project_name_upper);
		append(sb, "_");
		append(sb, file_name_upper);
		append(sb, "_GUARD");
		newline(sb);
	} else {
		append(sb, "// ");
		append(sb, project_name_upper);
		append(sb, "_");		
		append(sb, file_name_upper);
		append(sb, "_GUARD");
		newline(sb);
		append(sb, "#endif");
	}
	
	newline(sb);
}

}

#endif
