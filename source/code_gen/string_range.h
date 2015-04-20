
#ifndef LIBAXL_STRING_RANGE_GUARD
#define LIBAXL_STRING_RANGE_GUARD

#include "../string_buffer.h"
#include "stdio.h"

namespace libaxl {
	struct string {
		const char* first;
		size_t length;
	};

	inline
	size_t length(string s) {
		return s.length;
	}

	inline
	size_t to_c_string(string s, char* out, size_t max_length) {
		size_t initial_max_length = max_length;
		while(max_length > 1 && s.length > 0) {
			*out = *s.first;

			++out;
			++s.first;
			--max_length;
			--s.length;
		}

		*out = '\0';

		return initial_max_length - max_length;
	}

	inline
	void append(string_buffer& sb, string s) {
		assert(s.length <= 2147483647U);

		append(sb, s.first, (int)s.length);
	}

	inline
	void print(string s) {
		char tmp[64];
		size_t i = 0;

		for(; i < s.length; i += 63U) {
			size_t chunk_length = s.length - i;
			if(chunk_length > 63U)
				chunk_length = 63U;

			memcpy(tmp, s.first + i, chunk_length);
			tmp[chunk_length] = '\0';

			printf("%s", tmp);
		}
	}

}

#endif
