
#ifndef LIBAXL_NODE_ID_GUARD
#define LIBAXL_NODE_ID_GUARD

namespace libaxl {
enum node_id_enum {
	node_id_void = 0,

	//atoms
	node_id_constant,
	node_id_variable,
	node_id_type,
	node_id_name,

	//declarations
	node_id_struct_decl,
	node_id_func_decl,

	//statements
	node_id_assign,
	node_id_decl_assign,
	node_id_decl,
	node_id_return,
	node_id_while,
	node_id_if,
	node_id_else,

	//expressions
	node_id_add,
	node_id_sub,
	node_id_mul,
	node_id_div,

	node_id_dereference,
	node_id_address_of,
	node_id_equality,
	node_id_inequality,
	node_id_member_access,

	node_id_increment,
	node_id_decrement,

	node_id_func_call,

	//markers
	node_id_open,
	node_id_close,
};
}

#endif
