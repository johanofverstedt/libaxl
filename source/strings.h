
#ifndef LIBAXL_STRINGS_GUARD
#define LIBAXL_STRINGS_GUARD

#include "util.h"
#include "arena.h"

namespace libaxl {
struct const_string {
	const char* array;
	arena* arena;
	index_type count;
	index_type stride;
};
struct string {
	char *array;
	arena* arena;
	index_type count;
	index_type stride;
};

inline
const_string wrap_cstring(arena* arena, const char* str) {
	const_string result;

	assert(str != nullptr);

	result.array = str;
	result.arena = arena;
	result.count = strlen(str);
	result.stride = 1;

	return result;
}

inline
const_string make_const_string(arena* arena, const char* str) {
	const_string result;

	assert(arena != nullptr);

	if(str != nullptr) {
		result.count = strlen(str);
		char* mem = allocate<char>(arena, result.count);
		result.array = (const char*)mem;
		result.arena = arena;
		result.stride = 1;

		memcpy(mem, str, result.count);
	} else {
		result.count = 0;
		result.array = nullptr;
		result.arena = arena;
		result.stride = 1;
	}

	return result;
}

inline
const_string make_const_string(arena* arena, const char* str, index_type len) {
	const_string result;

	assert(arena != nullptr);

	if (str != nullptr) {
		result.count = len;
		char* mem = allocate<char>(arena, result.count);
		result.array = (const char*)mem;
		result.arena = arena;
		result.stride = 1;

		memcpy(mem, str, result.count);
	}
	else {
		result.count = 0;
		result.array = nullptr;
		result.arena = arena;
		result.stride = 1;
	}

	return result;
}

inline
string make_string(arena* arena, const char* str) {
	string result;

	assert(arena != nullptr);

	if(str != nullptr) {
		result.count = strlen(str);
		result.array = allocate<char>(arena, result.count);
		result.arena = arena;
		result.stride = 1;

		memcpy(result.array, str, result.count);
	} else {
		result.count = 0;
		result.array = nullptr;
		result.arena = arena;
		result.stride = 1;
	}

	return result;	
}

inline
string make_uninitialized_string(arena *arena, index_type count) {
	string result;

	assert(arena != nullptr);
	assert(count >= 0);

	result.array = allocate<char>(arena, count);
	result.arena = arena;
	result.count = count;
	result.stride = 1;

	return result;
}

inline
index_type length(const_string s) {
	return s.count;
}

inline
index_type length(string s) {
	return s.count;
}

inline
const_string take(const_string s, index_type count) {
	const_string result;

	assert(count <= s.count);

	result.array = s.array;
	result.arena = s.arena;
	result.count = count;
	result.stride = s.stride;

	return result;
}

inline
string take(string s, index_type count) {
	string result;

	assert(count <= s.count);

	result.array = s.array;
	result.arena = s.arena;
	result.count = count;
	result.stride = s.stride;

	return result;
}

inline
const_string take_at_most(const_string s, index_type count) {
	const_string result;

	result.array = s.array;
	result.arena = s.arena;
	result.count = count;
	if(count > s.count)
		result.count = s.count;
	result.stride = s.stride;

	return result;
}

inline
string take_at_most(string s, index_type count) {
	string result;

	result.array = s.array;
	result.arena = s.arena;
	result.count = count;
	if(count > s.count)
		result.count = s.count;
	result.stride = s.stride;

	return result;
}

inline
const_string reverse(const_string s) {
	const_string result;

	result.array = s.array + (s.count - 1) * s.stride;
	result.arena = s.arena;
	result.count = s.count;
	result.stride = -s.stride;

	return result;
}

inline
string reverse(string s) {
	string result;

	result.array = s.array + (s.count - 1) * s.stride;
	result.arena = s.arena;
	result.count = s.count;
	result.stride = -s.stride;

	return result;
}

inline
const_string operator+(const_string a, const_string b) {
	const_string result;

	result.count = length(a) + length(b);
	result.array = allocate<const char>(a.arena, result.count);
	result.arena = a.arena;
	result.stride = 1;

	return result;
}

}

#endif
