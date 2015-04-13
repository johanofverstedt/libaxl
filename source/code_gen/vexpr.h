
#ifndef LIBAXL_VEXPR_GUARD
#define LIBAXL_VEXPR_GUARD

#include "expr.h"

namespace libaxl {
struct vexpr {
	expr e;
	arena* arena;
};

inline
vexpr make_vector_expr(expr e, arena* arena) {
	vexpr result;

	result.e = e;
	result.arena = arena;

	return result;
}
}

#endif
