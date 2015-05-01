
/**
 *
 *  str_type.h
 *
 *  the string primitive in this library is a string reference type (str)
 *  which either contains the string itself if it's as short as the
 *  size of a pointer or a pointer to the string itself, located in memory.
 *
 *  since the str type is a very fat pointer type it will not be suitable for
 *  every need, but designed for safety, easy comparisons and
 *  compatibility with a large hashed string table for unique storage
 *  of strings that may occur repeatedly such as in a program text.
 *
 */

#ifndef STRING_LIBRARY_STR_TYPE_GUARD
#define STRING_LIBRARY_STR_TYPE_GUARD

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "types.h"
#include "hash.h"

#define STRING_TO_CSTRING(s) (cstring)(((s).info.length <= 8) ? (s).sso_buf : (s).ptr)

namespace string_library {
struct str_info {
	u32 length;
	u32 hash;
};

struct str {
	str_info info;
	union {
		cstring ptr;
		char    sso_buf[sizeof(cstring)];
	};
};

inline
str_info make_str_info(cstring s) {
	str_info result;

	result.hash = hash_u32_zero_terminated(s, &result.length);

	return result;
}

inline
str_info make_str_info(cstring s, u32 length) {
	str_info result;

	result.length = length;
	result.hash = hash_u32(s, length);

	return result;
}

inline
str make_string(cstring s, str_info info) {
	str result;

	result.info = info;
	if(info.length <= sizeof(cstring)) {
		memcpy(result.sso_buf, s, info.length);
	} else {
		result.ptr = s;
	}

	return result;
}

inline
str make_string(cstring s, u32 length) {
	str result;
	str_info info;

	info = make_str_info(s, length);

	result = make_string(s, info);

	return result;
}

inline
str make_string(cstring s) {
	str result;
	str_info info;

	info = make_str_info(s);

	result = make_string(s, info);

	return result;
}

inline
str make_empty_string() {
	str result;

	result.info.length = 0;
	result.info.hash = hash_u32("", 0U);

	return result;
}

inline
u32 length(str s) {
	u32 result = s.info.length;

	return result;
}

inline
u32 hash(str s) {
	u32 result = s.info.hash;

	return result;
}

inline
cstring string_to_cstring(str* s) {
	cstring result;

	if(s->info.length <= sizeof(cstring)) {
		result = (cstring)s->sso_buf;
	} else {
		result = s->ptr;
	}

	return result;	
}

inline
cstring string_to_cstring(str* s, u32 index) {
	cstring result;

	assert(index <= s->info.length);

	if(s->info.length < sizeof(cstring)) {
		result = (cstring)s->sso_buf + index;
	} else {
		result = s->ptr + index;
	}

	return result;	
}

inline
void print(str s) {
	cstring ptr = STRING_TO_CSTRING(s);

	fwrite(ptr, sizeof(char), s.info.length, stdout);
}

inline
bool operator==(str s1, str s2) {
	if(s1.info.hash != s2.info.hash)
		return false;
	if(s1.info.length != s2.info.length)
		return false;
	cstring s1_ptr = STRING_TO_CSTRING(s1);
	cstring s2_ptr = STRING_TO_CSTRING(s2);

	for (u32 i = 0; i < s1.info.length; ++i) {
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
	u32 len = (s1.info.length < s2.info.length) ? s1.info.length : s2.info.length;
	u32 i = 0;
	cstring s1_ptr = STRING_TO_CSTRING(s1);
	cstring s2_ptr = STRING_TO_CSTRING(s2);

	while(i < len) {
		if(s1_ptr[i] != s2_ptr[i])
			return s1_ptr[i] < s2_ptr[i];
		++i;
	}

	return s1.info.length < s2.info.length;
}

inline
bool operator>(str s1, str s2) {
	u32 len = (s1.info.length < s2.info.length) ? s1.info.length : s2.info.length;
	u32 i = 0;
	cstring s1_ptr = STRING_TO_CSTRING(s1);
	cstring s2_ptr = STRING_TO_CSTRING(s2);

	while(i < len) {
		if(s1_ptr[i] != s2_ptr[i])
			return s1_ptr[i] > s2_ptr[i];
		++i;
	}

	return s1.info.length > s2.info.length;
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
