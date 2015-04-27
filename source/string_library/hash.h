
/**
 *  string hashing functions
 *  
 *  implementation of FMV-algorithm which is a fast
 *  reliable non-crypto algorithm of reasonable quality
 *  from what my research tells me. works well enough
 *  for the purposes of this library.
 */

#ifndef STRING_LIBRARY_HASH_GUARD
#define STRING_LIBRARY_HASH_GUARD

#include "types.h"

namespace string_library {
//
// constants defined by the FMV-hashing algorithm
//
const u64 FMV_OFFSET = 14695981039346656037ULL;
const u64 FMV_PRIME = 1099511628211ULL;

//
// string hashing functions
//

inline
u64 hash_u64(cstring in, u32 length) {
	u64 hc = FMV_OFFSET;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	for(u32 i = 0; i < length; ++i) {
		hc *= FMV_PRIME;
		(*hc_byte_ptr) ^= (byte)in[i];
	}
    return hc;
}

inline
u32 hash_u32(cstring in, u32 length) {
	u32 result;
	u64 large_hash = hash_u64(in, length);

	result = (u32)large_hash;

	return result;
}

inline
u64 hash_u64(cstring in, u64 length) {
	u64 hc = FMV_OFFSET;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	for(u64 i = 0; i < length; ++i) {
		hc *= FMV_PRIME;
		(*hc_byte_ptr) ^= (byte)in[i];
	}
    return hc;
}

inline
u32 hash_u32(cstring in, u64 length) {
	u32 result;
	u64 large_hash = hash_u64(in, length);

	result = (u32)large_hash;

	return result;
}

//
// zero-terminated hashing functions.
// must only be used on trusted input for the
// usual safety reasons.
//
// an optional length pointer parameter may
// be supplied if knowledge of the length of
// the string is desired as a byproduct of
// calculating the hash.
//

inline
u64 hash_u64_zero_terminated(cstring in, u32* length) {
	u64 hc = FMV_OFFSET;
	u32 len = 0;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	
	while(in[len] != '\0') {
		hc *= FMV_PRIME;
		(*hc_byte_ptr) ^= (byte)in[len];
		++len;
	}

	if(length != nullptr)
		*length = len;
    return hc;
}

inline
u32 hash_u32_zero_terminated(cstring in, u32* length) {
	u32 result;
	u64 large_hash = hash_u64_null_terminated(in, length);

	result = (u32)large_hash;

	return result;
}

inline
u64 hash_u64_zero_terminated(cstring in, u64* length) {
	u64 hc = FMV_OFFSET;
	u64 len = 0;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	
	while(in[len] != '\0') {
		hc *= FMV_PRIME;
		(*hc_byte_ptr) ^= (byte)in[len];
		++len;
	}

	if(length != nullptr)
		*length = len;
    return hc;
}

inline
u32 hash_u32_zero_terminated(cstring in, u64* length) {
	u32 result;
	u64 large_hash = hash_u64_null_terminated(in, length);

	result = (u32)large_hash;

	return result;
}
}

#endif
