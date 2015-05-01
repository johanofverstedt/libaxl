
#ifndef STRING_LIBRARY_STRING_BUFFER_GUARD
#define STRING_LIBRARY_STRING_BUFFER_GUARD

#include "str_type.h"
#include "stack.h"

namespace string_library {
using string_buffer = stack;

inline
str to_string(string_buffer* sb) {
	str result;

	result = make_string((cstring)stack_ptr(sb, 0), length(sb));

	return result;
}

inline
void append(string_buffer* sb, str s) {
	push(sb, STRING_TO_CSTRING(s), length(s));
}

inline
void append_line(string_buffer* sb, str s) {
	append(sb, s);
	push(sb, '\n');
}
inline
void append_line_indent(string_buffer* sb, str s, u32 indent) {
	append(sb, s);
	push(sb, '\n');
	push_dup(sb, (u8)(' '), indent);
}

inline
void append(string_buffer* sb, u64 number) {
	char* first = (char*)stack_top_ptr(sb);

	//push the digits in reverse order

	do {
		char digit = '0' + (char)(number % 10ULL);
		number /= 10ULL;
		push(sb, digit);
	} while(number);

	//reverse

	char* end = (char*)stack_top_ptr(sb);

	while(first < end) {
		--end;

		char tmp = *first;
		*first = *end;
		*end = tmp;

		++first;
	}
}

inline
void append(string_buffer* sb, u32 number) {
	append(sb, (u64)number);
}

inline
void append(string_buffer* sb, i64 number) {
	char* first = (char*)stack_top_ptr(sb);

	//push the digits in reverse order

	if(number < 0LL) {
		push(sb, '-');

		if(number == -9223372036854775808LL)
			append(sb, 9223372036854775808ULL);
		else
			append(sb, (u64)(-number));
	} else {
		append(sb, (u64)number);
	}
}

inline
void append(string_buffer* sb, i32 number) {
	append(sb, (i64)number);
}

}

#endif
