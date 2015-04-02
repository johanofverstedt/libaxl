
#ifndef LIBAXL_UTIL_GUARD
#define LIBAXL_UTIL_GUARD

#include <cassert>
#include <cstring>

#define LIBAXL_ERROR(msg) if(msg) assert(false, (msg)); else assert(false)

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
