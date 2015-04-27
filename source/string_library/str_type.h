
/**
 *
 *  str_type.h
 *
 *  the string primitive in this library is a string reference type (string_ref)
 *  which either contains the string itself if it's as short as the
 *  size of a pointer or a pointer to the string itself, located in memory.
 *
 *  since the string_ref type is a fat pointer type it will not be suitable for
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
	if(info.length < sizeof(cstring)) {
		memcpy(result.sso_buf, s, info.length);
		result.sso_buf[info.length] = '\0';
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
cstring string_to_cstring(str s) {
	cstring result;

	if(s.info.length < sizeof(cstring)) {
		result = &s.sso_buf[0];
	} else {
		result = s.ptr;
	}

	assert(result[s.info.length] == '\0');

	return result;	
}

inline
cstring string_to_cstring(str s, u32 index) {
	cstring result;

	assert(index <= s.info.length);

	if(s.info.length < sizeof(cstring)) {
		result = &s.sso_buf[index];
	} else {
		result = s.ptr + index;
	}

	assert(result[s.info.length - index] == '\0');

	return result;	
}

inline
u32 read_string(str s, u32 index, mutable_cstring out, u32 out_size) {
	u32 read_count;

	assert(out_size > 0);

	cstring in_ptr = string_to_cstring(s, index);

	read_count = (s.info.length + 1) - index;
	if(read_count > out_size) {
		read_count = out_size - 1;
		out[out_size - 1] = '\0';
	}

	memcpy(out, in_ptr, read_count);

	return read_count;
}

inline
void print(str s) {
	cstring ptr = string_to_cstring(s);

	printf("%s", ptr);	
}

inline
bool operator==(str s1, str s2) {
	if(s1.info.hash != s2.info.hash)
		return false;
	if(s1.info.length != s2.info.length)
		return false;
	cstring s1_ptr = string_to_cstring(s1);
	cstring s2_ptr = string_to_cstring(s2);

	for(u32 i = 0; i < s1.info.length) {
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
	cstring s1_ptr = string_to_cstring(s1);
	cstring s2_ptr = string_to_cstring(s2);

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
	cstring s1_ptr = string_to_cstring(s1);
	cstring s2_ptr = string_to_cstring(s2);

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
