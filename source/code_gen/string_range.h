
#ifndef LIBAXL_STRING_RANGE_GUARD
#define LIBAXL_STRING_RANGE_GUARD

#include "../string_buffer.h"
#include "stdio.h"

namespace libaxl {
	struct string_range {
		const char* first;
		u32 length; //Should change to size_t
	};

	inline
	u32 length(string_range s) {
		return s.length;
	}

	inline
	u32 to_c_string(string_range s, char* out, u32 max_length) {
		u32 initial_max_length = max_length;
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
	void append(string_buffer& sb, string_range s) {
		assert(s.length <= 2147483647U);

		append(sb, s.first, (i32)s.length);
	}

	inline
	void print(string_range s) {
		char tmp[64];
		u32 i = 0;

		for(; i < s.length; i += 63U) {
			u32 chunk_length = s.length - i;
			if(chunk_length > 63U)
				chunk_length = 63U;

			memcpy(tmp, s.first + i, (size_t)chunk_length);
			tmp[chunk_length] = '\0';

			printf("%s", tmp);
		}
	}

}

#endif
