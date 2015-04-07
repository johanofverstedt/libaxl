
#ifndef LIBAXL_CONST_EXPR_GUARD
#define LIBAXL_CONST_EXPR_GUARD

namespace libaxl {
template <typename T>
struct const_expr {
	T value;

	ALWAYS_INLINE
	auto operator[](index_type index) -> T {
		return value;
	}
};	

template <typename T>
inline
index_type length(const_expr<T> e) {
	return 2147483647;
}

//
//  Factory function
//
template <typename T>
inline
const_expr<T> constant(T value) {
	const_expr<T> result;

	result.value = value;

	return result;
}

}

// LIBAXL_CONST_EXPR_GUARD
#endif
