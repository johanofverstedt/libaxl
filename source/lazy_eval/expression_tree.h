
#ifndef LIBAXL_EXPRESSION_TREE_GUARD
#define LIBAXL_EXPRESSION_TREE_GUARD

#include "util.h"
#include "vectors.h"

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

template <typename T1, typename T2>
struct add_expr {
	T1 left;
	T2 right;

	ALWAYS_INLINE
	auto operator[](index_type index) -> decltype(left[index] + right[index]) {
		return left[index] + right[index];
	}
};

template <typename T1, typename T2>
inline
index_type length(add_expr<T1, T2> e) {
	return minimum(length(e.left), length(e.right));
}

template <typename T1, typename T2>
struct mul_expr {
	T1 left;
	T2 right;

	ALWAYS_INLINE
	auto operator[](index_type index) -> decltype(left[index] * right[index]) {
		return left[index] * right[index];
	}
};

template <typename T1, typename T2>
inline
index_type length(mul_expr<T1, T2> e) {
	return minimum(length(e.left), length(e.right));
}

template <typename T1, typename T2>
inline
auto operator+(T1 a, T2 b) -> add_expr<T1, T2> {
	add_expr<T1, T2> result;

	result.left = a;
	result.right = b;

	return result;
}

template <typename T1, typename T2>
inline
auto operator*(T1 a, T2 b) -> mul_expr<T1, T2> {
	mul_expr<T1, T2> result;

	result.left = a;
	result.right = b;

	return result;
}

template <typename E>
inline
auto eval(E e, arena* arena) -> vector<decltype(e[0])> {
	using value_type = decltype(e[0]);
	vector<value_type> result;

	result.arena = arena;
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
