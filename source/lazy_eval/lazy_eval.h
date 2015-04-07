
#ifndef LIBAXL_LAZY_EVAL_GUARD
#define LIBAXL_LAZY_EVAL_GUARD

#include "../util.h"
#include "../vectors.h"
#include "../arena.h"

//
//  Operations
//

#include "const_expr.h"
#include "add_expr.h"
#include "sub_expr.h"
#include "mul_expr.h"
#include "div_expr.h"
#include "square_expr.h"

namespace libaxl {

template <typename E>
inline
auto eval(E e, arena* arena) -> vector<decltype(e[0])> {
	using value_type = decltype(e[0]);
	vector<value_type> result;

	result.count = length(e);
	result.array = allocate<value_type>(arena, result.count);
	result.stride = 1;

	for(index_type i = 0; i < result.count; ++i) {
		auto value = e[i];
		result.array[i] = value;
	}

	return result;
}
}

#endif
