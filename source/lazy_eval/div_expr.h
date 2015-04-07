
#ifndef LIBAXL_DIV_EXPR_GUARD
#define LIBAXL_DIV_EXPR_GUARD

namespace libaxl {
template <typename T1, typename T2>
struct div_expr {
	T1 left;
	T2 right;

	ALWAYS_INLINE
	auto operator[](index_type index) -> decltype(left[index] / right[index]) {
		return left[index] / right[index];
	}
};

template <typename T1, typename T2>
inline
index_type length(div_expr<T1, T2> e) {
	return minimum(length(e.left), length(e.right));
}

template <typename T1, typename T2>
inline
div_expr<T1, T2> operator/(T1 a, T2 b) {
	div_expr<T1, T2> result;

	result.left = a;
	result.right = b;

	return result;
}
}

// LIBAXL_DIV_EXPR_GUARD
#endif
