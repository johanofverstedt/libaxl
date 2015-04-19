
#ifndef LIBAXL_STATEMENT_GUARD
#define LIBAXL_STATEMENT_GUARD

#include "variable.h"
#include "expr.h"

namespace libaxl {
enum statement_id_enum {
	statement_id_void,
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

struct expr_statement_pair {
	expr e;
	statement s;
};

struct scope {
	int32_t statement_count;
	statement* statements;
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
		case statement_id_while:
			result.data = allocate<expr_statement_pair>(arena, 1);
			break;
		default:
			assert(false);
	}

	return result;
}

inline
statement make_scope_statement(arena* arena, int statement_count) {
	statement result;

	result.id = statement_id_scope;

	scope* scope_ptr = allocate<scope>(arena, 1);
	result.data = scope_ptr;

	scope_ptr->statement_count = statement_count;
	scope_ptr->statements = allocate<statement>(arena, statement_count);

	for(int32_t i = 0; i < statement_count; ++i)
		scope_ptr->statements[i].id = statement_id_void;

	return result;
}

inline
void codegen(cg_context* context, statement s) {
	string_buffer& sb = context->sb;

	switch(s.id) {
		case statement_id_assignment:
		codegen(context, ((expr*)s.data)[0]);
		append(sb, " = ");
		codegen(context, ((expr*)s.data)[1]);
		append(sb, ";");
		break;
		
		case statement_id_return:
		append(sb, "return ");
		codegen(context, ((expr*)s.data)[0]);
		append(sb, ";");
		break;

		case statement_id_decl_assign_var:
		codegen(context, ((decl_assign_pair*)s.data)->var, true);
		append(sb, " = ");
		codegen(context, ((decl_assign_pair*)s.data)->e);
		append(sb, ";");
		break;

		case statement_id_while:
		append(sb, "while(");
		codegen(context, ((expr_statement_pair*)s.data)->e);
		append(sb, ") ");
		codegen(context, ((expr_statement_pair*)s.data)->s);
		break;

		default:
		append(sb, ";"); //Temporary
		break;
	}
	
}

}

#endif
