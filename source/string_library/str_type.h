
/**
 *
 *  str_type.h
 *
 *  the string primitive in this library is a string reference type (str)
 *  which either contains the string itself if it's as short as the
 *  size of a pointer or a pointer to the string itself, located in memory.
 *  it also contains the length of the string.
 */

#ifndef STRING_LIBRARY_STR_TYPE_GUARD
#define STRING_LIBRARY_STR_TYPE_GUARD

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "hash.h"

// macro for extracting a cstring pointer from a str-object
#define STRING_TO_CSTRING(s) (cstring)(((s).length <= sizeof(cstring)) ? (s).sso_buf : (s).ptr)

// macro for converting a string literal to a str-object using the
// array-size finding pattern (excluding the zero-terminator)
#define MAKE_STRING_FROM_LITERAL(literal) make_string((literal), (sizeof(literal))/sizeof((literal)[0])-1)

namespace string_library {
struct str {
	union {
		cstring ptr;
		char    sso_buf[sizeof(cstring)];
	};
	u32 length;

	inline
	char operator[](u32 index) {
		assert(index < length);
		
		return length <= sizeof(cstring) ?
			sso_buf[index] :
			ptr[index];
	}
};

inline
str make_string(cstring s, u32 length) {
	str result;

	if(length <= sizeof(cstring)) {
		memcpy(result.sso_buf, s, length);
	} else {
		result.ptr = s;
	}

	result.length = length;

	return result;
}

inline
str make_string(cstring s) {
	str result;

	size_t len = strlen(s);

	assert(len <= 4294967295U);

	result = make_string(s, (u32)len);

	return result;
}

inline
str make_empty_string() {
	str result;

	result.length = 0;

	return result;
}

inline
u32 length(str s) {
	u32 result = s.length;

	return result;
}

inline
u32 hash(str s) {
	u32 result = hash_u32(STRING_TO_CSTRING(s), s.length);

	return result;
}

inline
cstring string_to_cstring(str* s) {
	cstring result;

	if(s->length <= sizeof(cstring)) {
		result = (cstring)s->sso_buf;
	} else {
		result = s->ptr;
	}

	return result;	
}

inline
cstring string_to_cstring(str* s, u32 index) {
	cstring result;

	assert(index <= s->length);

	if(s->length < sizeof(cstring)) {
		result = (cstring)s->sso_buf + index;
	} else {
		result = s->ptr + index;
	}

	return result;	
}

inline
str take(str s, u32 count) {
	str result;

	assert(count <= length(s));

	result = make_string(STRING_TO_CSTRING(s), count);

	return result;
}

inline
str drop(str s, u32 count) {
	str result;

	auto len = length(s);

	assert(count <= len);

	result = make_string(STRING_TO_CSTRING(s) + count, len - count);

	return result;
}

inline
void print(str s) {
	cstring ptr = STRING_TO_CSTRING(s);

	fwrite(ptr, sizeof(char), s.length, stdout);
}

inline
bool operator==(str s1, str s2) {
	if(s1.length != s2.length)
		return false;
	cstring s1_ptr = STRING_TO_CSTRING(s1);
	cstring s2_ptr = STRING_TO_CSTRING(s2);

	for (u32 i = 0; i < s1.length; ++i) {
		if(s1_ptr[i] != s2_ptr[i])
			return false;
	}

	return true;
}

inline
bool operator!=(str s1, str s2) {
	return !(s1 == s2);
}

inline
bool operator<(str s1, str s2) {
	u32 len = (s1.length < s2.length) ? s1.length : s2.length;
	u32 i = 0;
	cstring s1_ptr = STRING_TO_CSTRING(s1);
	cstring s2_ptr = STRING_TO_CSTRING(s2);

	while(i < len) {
		if(s1_ptr[i] != s2_ptr[i])
			return s1_ptr[i] < s2_ptr[i];
		++i;
	}

	return s1.length < s2.length;
}

inline
bool operator>(str s1, str s2) {
	u32 len = (s1.length < s2.length) ? s1.length : s2.length;
	u32 i = 0;
	cstring s1_ptr = STRING_TO_CSTRING(s1);
	cstring s2_ptr = STRING_TO_CSTRING(s2);

	while(i < len) {
		if(s1_ptr[i] != s2_ptr[i])
			return s1_ptr[i] > s2_ptr[i];
		++i;
	}

	return s1.length > s2.length;
}

inline
bool operator<=(str s1, str s2) {
	return !(s1 > s2);
}

inline
bool operator>=(str s1, str s2) {
	return !(s1 < s2);
}
}

#endif
