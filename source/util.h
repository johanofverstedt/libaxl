
#ifndef LIBAXL_UTIL_GUARD
#define LIBAXL_UTIL_GUARD

#include <cassert>
#include <cstring>

namespace libaxl {
template <typename T>
inline
T minimum(T first, T second) {
	return (second < first) ? second : first;
}
template <typename T>
inline
T maximum(T first, T second) {
	return (first > second) ? first : second;
}
}

#endif
