
#ifndef LIBAXL_PROGRAM_GUARD
#define LIBAXL_PROGRAM_GUARD

#include "syntax_tree.h"
#include "symbol_table.h"
#include "node_id.h"
#include "string_range.h"

namespace libaxl {
	struct program {
		syntax_tree*  tree;
		symbol_table* name_table;
		symbol_table* type_table;
	};

	inline
	void push_variable(program p, string_range name) {
		symbol s;

		s.name = name.first;
		s.length = length(name);

		int index = p.table->add_symbol(p.name_table, s);

		node n = make_node(
			node_id_variable,
			index);

		push(p.tree, &n);
	}

	inline
	void push_type(program p, string_range name) {
		symbol s;

		s.name = name.first;
		s.length = length(name);

		int index = p.table->add_symbol(p.type_table, s);

		node n = make_node(
			node_id_type,
			index);

		push(p.tree, &n);		
	}

	inline
	void push_decl(program p, string_range name, string_range type_name) {
		node n = make_node(
			node_id_decl);

		push(p.tree, &n);

		push_variable(p, name);
		push_type(p, type_name);
	}

	inline
	void push_node(program p, int id) {
		node n = make_node(id);
		push(p.tree, &n);
	}
}

#endif
