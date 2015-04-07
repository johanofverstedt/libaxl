
#ifndef LIBAXL_VECTOR_PAIR_GUARD
#define LIBAXL_VECTOR_PAIR_GUARD

#include "util.h"
#include "vectors.h"

namespace libaxl {
template <typename T>
struct vector_pair {
	vector<T> v[2];
};

template <typename T>
inline
vector_pair<T> make_vector_pair(vector<T> first, vector<T> second) {
	vector_pair<T> result;

	result.v[0] = first;
	result.v[1] = second;

	return result;
}

template <typename T>
inline
vector<T> first(vector_pair<T> vp) {
	return vp.v[0];
}

template <typename T>
inline
vector<T> second(vector_pair<T> vp) {
	return vp.v[1];
}

template <typename T>
inline
vector<T> pair_indexed(vector_pair<T> vp, index_type index) {
	assert(index == 0 || index == 1);

	return vp.v[index];
}

template <typename T>
inline
vector_pair<T> swap_pair(vector_pair<T> vp) {
	vector tmp = vp.v[0];
	vp.v[0] = vp.v[1];
	vp.v[1] = tmp;

	return vp;
}

template <typename T>
inline
void swap(vector_pair<T>& a, vector_pair<T>& b) {
	vector tmp = a.v[0];
	a.v[0] = b.v[0];
	b.v[0] = tmp;

	tmp = a.v[1];
	a.v[1] = b.v[1];
	b.v[1] = tmp;
}

template <typename T>
inline
index_type length(vector_pair<T> vp) {
	return length(vp.v[0]) + length(vp.v[1]);
}

template <typename T>
inline
vector_pair<T> reverse(vector_pair<T> vp) {
	vector_pair<T> result = make_vector_pair(
		reverse(second(vp)),
		reverse(first(vp)));

	return result;
}

template <typename T>
inline
vector_pair<T> take_at_most(vector_pair<T> vp, index_type count) {
	vector_pair<T> result;

	result.v[0] = take_at_most(first(vp), count);
	result.v[1] = take_at_most(second(vp), count - length(result.v[0]));

	return result;
}

template <typename T>
inline
vector_pair<T> take(vector_pair<T> vp, index_type count) {
	vector_pair<T> result = take_at_most(vp, count);

	assert(length(first(result)) + length(second(result)) == count);

	return result;
}

template <typename T>
inline
vector_pair<T> drop_at_most(vector_pair<T> vp, index_type count) {
	vector_pair<T> result;

	result.v[0] = drop_at_most(first(vp), count);
	result.v[1] = drop_at_most(second(vp), count - (length(first(vp)) - length(first(result))));

	return result;
}

template <typename T>
inline
vector_pair<T> drop(vector_pair<T> vp, index_type count) {
	vector_pair<T> result = drop_at_most(vp, count);

	assert(length(vp) - length(result) == count);

	return result;
}

} //namespace libaxl

// LIBAXL_VECTOR_PAIR_GUARD
#endif
