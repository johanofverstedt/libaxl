
#ifndef LIBAXL_EXPR_CODE_GEN_GUARD
#define LIBAXL_EXPR_CODE_GEN_GUARD

#include "../arena.h"
#include "../stack_arena.h"
#include "../string_buffer.h"

#include "type.h"
#include "value.h"

namespace libaxl {
enum expr_id {
	//Values
	expr_id_constant,
	expr_id_variable,

	//Unary
	expr_id_logical_not,
	expr_id_negate,
	expr_id_address_of,
	expr_id_dereference,
	expr_id_square,
	expr_id_cube,
	expr_id_sqrt,
	expr_id_log10,

	expr_id_add,
	expr_id_sub,
	expr_id_mul,
	expr_id_div,
	expr_id_mod,
	expr_id_pow,
	
	expr_id_subscript,

	expr_id_bitwise_and,
	expr_id_bitwise_or,

	expr_id_logical_and,
	expr_id_logial_or,
};

struct expr {
	void* data;
	arena* expr_arena;

	int32_t id;
};

inline
value eval(expr e, arena* arena);
inline
value eval(expr e, arena* arena, int index);

value eval_constant(expr* e, arena* arena) {
	value* value_ptr = (value*)e->data;

	return *value_ptr;
}

value eval_constant(expr* e, arena* arena, int index) {
	value* value_ptr = (value*)e->data;

	return *value_ptr;
}

inline
expr constant(arena* arena, value v) {
	expr result;

	value* inner = allocate<value>(arena, 1);
	*inner = v;

	result.data = inner;
	result.expr_arena = arena;
	result.id = expr_id_constant;

	return result;
}
inline
expr constant(arena* arena, int v) {
	expr result;

	value val = make_i32_value(arena, v);

	result = constant(arena, val);

	return result;
}
inline
expr constant(arena* arena, double v) {
	expr result;

	value val = make_double_value(arena, v);

	result = constant(arena, val);

	return result;
}

value unary_op_eval(expr* e, arena* arena, int index) {
	value result;

	expr* child = (expr*)e->data;

	result = eval(*child, arena);//, index);

	return result;
}

value eval_sqrt(expr* e, arena* arena, int index) {
	value result;

	value v = unary_op_eval(e, arena, index);

	if(v.type.id == type_info_double) {
		result = make_double_value(arena, sqrt(read_value<double>(v)));
	}

	return result;
}

void binary_op_eval(expr* e, arena* arena, int index, value* values_out) {
	expr* children = (expr*)e->data;

	values_out[0] = eval(children[0], arena, index);//children[0].eval_function(&children[0], arena, index);
	values_out[1] = eval(children[1], arena, index);//children[1].eval_function(&children[1], arena, index);
}
value eval_add(expr* e, arena* arena, int index) {
	value result;
	value values[2];

	binary_op_eval(e, arena, index, values);

	if (values[0].type.id == type_info_double && values[1].type.id == type_info_double) {
		result = make_double_value(arena, read_value<double>(values[0]) + read_value<double>(values[1]));
	}
	else if (values[0].type.id == type_info_i32 && values[1].type.id == type_info_i32) {
		result = make_i32_value(arena, read_value<i32>(values[0]) + read_value<i32>(values[1]));
	}

	return result;
}

value eval_mul(expr* e, arena* arena, int index) {
	value result;
	value values[2];

	binary_op_eval(e, arena, index, values);

	if (values[0].type.id == type_info_double && values[1].type.id == type_info_double) {
		result = make_double_value(arena, read_value<double>(values[0]) * read_value<double>(values[1]));
	}
	else if (values[0].type.id == type_info_i32 && values[1].type.id == type_info_i32) {
		result = make_i32_value(arena, read_value<i32>(values[0]) * read_value<i32>(values[1]));
	}

	return result;
}

inline
value eval(expr e, arena* arena) {
	switch(e.id) {
		case expr_id_constant:
			return eval_constant(&e, arena);
		case expr_id_add:
			return eval_add(&e, arena, 0);
		case expr_id_mul:
			return eval_mul(&e, arena, 0);
		case expr_id_sqrt:
			return eval_sqrt(&e, arena, 0);
	}
	assert(false);
	value error_value;
	return error_value;
}

inline
value eval(expr e, arena* arena, int index) {
	return eval(e, arena);
}

/*
struct add_expr : expr_base {
	expr left;
	expr right;

	virtual value eval(arena* arena, int index) {
		value x = left.ptr->eval(arena, index);
		value y = right.ptr->eval(arena, index);

		value result;

		if(x.type.id == type_info_double && y.type.id == type_info_double) {
			double xv = *(double*)x.value_ptr;
			double yv = *(double*)y.value_ptr;

			result = make_double_value(arena, xv + yv);
		} else if(x.type.id == type_info_int && y.type.id == type_info_int) {
			int xv = *(int*)x.value_ptr;
			int yv = *(int*)y.value_ptr;

			result = make_int_value(arena, xv + yv);
		}

		return result;
	}
};
*/

inline
expr make_unary_expr(expr x) {
	expr result;

	expr* child = allocate<expr>(x.expr_arena, 1);
	*child = x;

	result.data = child;

	return result;
}

inline
expr square_root(expr x) {
	expr result = make_unary_expr(x);

	result.expr_arena = x.expr_arena;
	result.id = expr_id_sqrt;

	return result;
}

inline
expr make_binary_expr(expr a, expr b) {
	expr result;

	expr* children = allocate<expr>(a.expr_arena, 2);
	children[0] = a;
	children[1] = b;

	result.data = children;

	return result;
}

inline
expr operator+(expr a, expr b) {
	expr result = make_binary_expr(a, b);

	result.expr_arena = a.expr_arena;
	result.id = expr_id_add;

	return result;
}

inline
expr operator*(expr a, expr b) {
	expr result = make_binary_expr(a, b);

	result.expr_arena = a.expr_arena;
	result.id = expr_id_mul;

	return result;
}

/*

template <typename V>
struct expr {
private:
	struct concept {
		virtual ~concept() = default;

		// Random access evaluation
		virtual V operator()(int index) = 0;
	};
	template <typename T>
	struct model : concept {
		model(T x) : data(x) {}
		virtual ~model() = default;

		virtual V operator()(int index) {
			return data(index);
		}

		T data;
	};

	concept* ptr;
public:
	template <typename T>
	expr()

	inline
	V operator()(int index) {

	}


};
*/
enum expr_type_enum {
	expr_type_unary,
	expr_type_binary,
};
enum unary_expr_op_enum {
	unary_expr_op_not,
	unary_expr_op_negate,
	unary_expr_op_square,
	unary_expr_op_cube,
	unary_expr_op_sqrt,
	unary_expr_op_abs,
	unary_expr_op_clamp,
	unary_expr_op_clamp_upper,
	unary_expr_op_clamp_lower,
};
enum bin_expr_op_enum {
	bin_expr_op_add,
	bin_expr_op_sub,
	bin_expr_op_mul,
	bin_expr_op_div,
	bin_expr_op_pow,
	bin_expr_op_log10,
	bin_expr_op_min,
	bin_expr_op_max,
};

}

#endif
