
#ifndef LIBAXL_STRING_BUFFER_GUARD
#define LIBAXL_STRING_BUFFER_GUARD

#include "util.h"
#include "arena.h"
#include "strings.h"

namespace libaxl {
struct string_buffer {
	char* memory;
	arena* arena;
	index_type size;
	index_type used;
};

inline
void space(string_buffer& buf, int count);

inline
index_type push(string_buffer& sb) {
	return sb.used;
}

inline
index_type pop(string_buffer& sb, index_type state) {
	auto new_used = sb.used;
	sb.used = state;
	return new_used - state;
}

inline
string_buffer make_string_buffer(arena* arena, index_type initial_size = 32) {
	assert(initial_size >= 0);
	assert(arena != nullptr);

	string_buffer result;

	if(initial_size > 0) {
		result.memory = allocate<char>(arena, initial_size);
		result.arena = arena;
	} else {
		result.memory = nullptr;
		result.arena = arena;
	}
	
	result.size = initial_size;
	result.used = 0;

	return result;
}

inline
string_buffer enlarge_string_buffer(string_buffer buf) {
	string_buffer result;

	if(buf.size > 0) {
		result.size = (buf.size * 3) / 2;
	} else {
		result.size = 1;
	}

	result.memory = allocate<char>(buf.arena, result.size);
	result.used = buf.used;

	return result;
}

inline
const_string to_string(string_buffer buf) {
	return make_const_string(buf.arena, buf.memory, buf.used);
}

inline
string_buffer& append(string_buffer& buf, const char* str) {
	while(*str) {
		buf.memory[buf.used] = *str;
		++buf.used;
		++str;
	}

	return buf;
}

inline
string_buffer& append(string_buffer& buf, const char* str, int length) {
	auto state = push(buf);
	append(buf, str);
	auto appended_length = buf.used - state;
	if(appended_length < length) {
		space(buf, length - appended_length);
	}
}

inline
string_buffer& append(string_buffer& buf, const_string str) {
	int slen = length(str);
	int read_index = 0;
	for(int i = 0; i < slen; ++i) {
		buf.memory[buf.used++] = str.array[read_index];
		read_index += str.stride;
	}

	return buf;
}

inline
void print(string_buffer& buf) {
	buf.memory[buf.used] = '\0';
	printf("%s", buf.memory);
}

template <typename T>
inline
string_buffer& operator+=(string_buffer& buf, T str) {
	return append(buf, str);
}

inline
void space(string_buffer& buf) {
	append(buf, " ");
}

inline
void space(string_buffer& buf, int count) {
	while(count--) {
		append(buf, " ");
	}
}

inline
void newline(string_buffer& buf) {
	append(buf, "\n");
}

inline
void newline(string_buffer& buf, int indent) {
	append(buf, "\n");
	space(buf, indent);
}

inline
void comma(string_buffer& buf) {
	append(buf, ", ");
}
}

// LIBAXL_STRING_BUFFER_GUARD
#endif
