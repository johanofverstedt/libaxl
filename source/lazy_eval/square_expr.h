
#ifndef LIBAXL_SQUARE_EXPR_GUARD
#define LIBAXL_SQUARE_EXPR_GUARD

namespace libaxl {
template <typename T>
struct square_expr {
	T child;

	ALWAYS_INLINE
	auto operator[](index_type index) -> decltype(child[index] * child[index]) {
		auto operand = child[index];
		return operand * operand;
	}
};

template <typename T>
inline
index_type length(square_expr<T> e) {
	return length(e.child);
}

template <typename T>
inline
square_expr<T> square(T x) {
	square_expr<T> result;

	result.child = x;

	return result;
}
}

// LIBAXL_SQUARE_EXPR_GUARD
#endif
