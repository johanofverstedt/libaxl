
#ifndef LIBAXL_VECTOR_PAIR_GUARD
#define LIBAXL_VECTOR_PAIR_GUARD

#include "util.h"
#include "vectors.h"

namespace libaxl {
struct vector_pair {
	vector v[2];
};

inline
vector_pair make_vector_pair(vector first, vector second) {
	vector_pair result;

	result.v[0] = first;
	result.v[1] = second;
}

inline
vector first(vector_pair vp) {
	return vp.v[0];
}

inline
vector second(vector_pair vp) {
	return vp.v[1];
}

inline
vector pair_indexed(vector_pair vp, int index) {
	assert(index == 0 || index == 1);

	return vp.v[index];
}

inline
vector_pair swap_pair(vector_pair vp) {
	vector tmp = vp.v[0];
	vp.v[0] = vp.v[1];
	vp.v[1] = tmp;

	return vp;
}

inline
void swap(vector_pair& a, vector_pair& b) {
	vector tmp = a.v[0];
	a.v[0] = b.v[0];
	b.v[0] = tmp;

	tmp = a.v[1];
	a.v[1] = b.v[1];
	b.v[1] = tmp;
}

inline
int length(vector_pair vp) {
	return length(vp.v[0]) + length(vp.v[1]);
}

inline
int width(vector_pair vp) {
	assert(vp.v[0].width == vp.v[1].width);

	return vp.v[0].width;
}

inline
vector_pair reverse(vector_pair vp) {
	vector_pair result = make_vector_pair(
		reverse(second(vp)),
		reverse(first(vp)));

	return result;
}

inline
vector_pair take(vector_pair vp, int count) {

	return vp;
}

inline
vector_pair drop(vector_pair vp, int count) {
	return vp;
}

} //namespace libaxl

// LIBAXL_VECTOR_PAIR_GUARD
#endif
