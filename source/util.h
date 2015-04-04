
#ifndef LIBAXL_UTIL_GUARD
#define LIBAXL_UTIL_GUARD

#include <cassert>
#include <cstring>
#include <cstdint>

#define LIBAXL_ERROR(msg) if(msg) assert(false, (msg)); else assert(false)

namespace libaxl {
//Types
using size_type = size_t;
using index_type = int32_t;
using scalar_type = double;

namespace detail {
	inline
	int adjust_alignment(size_type value) {
		assert(value >= 1U);
		switch(value) {
			case 1:
			return 1U;
			case 2:
			return 2U;
			case 3:
			case 4:
			return 4U;
			case 5:
			case 6:
			case 7:
			case 8:
			return 8U;
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			return 16U;
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
			case 32:
			return 32U;
			default:
			return 32U;
		}
	}
	inline
	size_type ptr_alignment_offset(unsigned char* ptr, size_type alignment) {
		size_type result = (alignment - ((size_type)(ptr) % alignment)) % alignment;
		return result;
	}	
}

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
