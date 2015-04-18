
#ifndef LIBAXL_STATEMENT_GUARD
#define LIBAXL_STATEMENT_GUARD

#include "expr.h"

namespace libaxl {
enum statement_id_enum {
	statement_id_assignment,
	statement_id_return,
	statement_id_decl_var,
	statement_id_decl_assign_var,
	statement_id_if,
	statement_id_else,
	statement_id_while,
	statement_id_scope,
};
struct statement {
	void* data;
	int id;
};

struct decl_assign_pair {
	variable var;
	expr e;
};

inline
statement make_statement(arena* arena, int id) {
	statement result;

	result.id = id;

	switch(id) {
		case statement_id_assignment:
			result.data = allocate<expr>(arena, 2);
			break;
		case statement_id_return:
			result.data = allocate<expr>(arena, 1);
			break;
		case statement_id_decl_var:
			result.data = allocate<variable>(arena, 1);
			break;
		case statement_id_decl_assign_var:
			result.data = allocate<decl_assign_pair>(arena, 1);
			break;
		default:
			assert(false);
	}

	return result;
}

inline
void codegen(cg_context* context, statement s) {
	string_buffer& sb = context->sb;
	append(sb, ";"); //Temporary
}

}

#endif
