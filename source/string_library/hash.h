
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
const u32 FMV_OFFSET_32 = 2166136261U;
const u32 FMV_PRIME_32 = 16777619U;
const u64 FMV_OFFSET_64 = 14695981039346656037ULL;
const u64 FMV_PRIME_64 = 1099511628211ULL;

//
// string hashing functions
//

inline
u64 hash_u64(cstring in, u32 length) {
	u64 hc = FMV_OFFSET_64;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	for(u32 i = 0; i < length; ++i) {
		hc *= FMV_PRIME_64;
		(*hc_byte_ptr) ^= (byte)in[i];
	}
    return hc;
}

inline
u32 hash_u32(cstring in, u32 length) {
	u32 hc = FMV_OFFSET_32;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	for(u32 i = 0; i < length; ++i) {
		hc *= FMV_PRIME_32;
		(*hc_byte_ptr) ^= (byte)in[i];
	}
    return hc;
}

inline
u64 hash_u64(cstring in, u64 length) {
	u64 hc = FMV_OFFSET_64;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	for(u64 i = 0; i < length; ++i) {
		hc *= FMV_PRIME_64;
		(*hc_byte_ptr) ^= (byte)in[i];
	}
    return hc;
}

inline
u32 hash_u32(cstring in, u64 length) {
	u32 hc = FMV_OFFSET_32;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	for(u64 i = 0; i < length; ++i) {
		hc *= FMV_PRIME_32;
		(*hc_byte_ptr) ^= (byte)in[i];
	}
    return hc;
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
	u64 hc = FMV_OFFSET_64;
	u32 len = 0;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	
	while(in[len] != '\0') {
		hc *= FMV_PRIME_64;
		(*hc_byte_ptr) ^= (byte)in[len];
		++len;
	}

	if(length != nullptr)
		*length = len;
    return hc;
}

inline
u32 hash_u32_zero_terminated(cstring in, u32* length) {
	u32 hc = FMV_OFFSET_32;
	u32 len = 0;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	
	while(in[len] != '\0') {
		hc *= FMV_PRIME_32;
		(*hc_byte_ptr) ^= (byte)in[len];
		++len;
	}

	if(length != nullptr)
		*length = len;
    return hc;
}

inline
u64 hash_u64_zero_terminated(cstring in, u64* length) {
	u64 hc = FMV_OFFSET_64;
	u64 len = 0;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	
	while(in[len] != '\0') {
		hc *= FMV_PRIME_64;
		(*hc_byte_ptr) ^= (byte)in[len];
		++len;
	}

	if(length != nullptr)
		*length = len;
    return hc;
}

inline
u32 hash_u32_zero_terminated(cstring in, u64* length) {
	u32 hc = FMV_OFFSET_32;
	u64 len = 0;
	byte_ptr hc_byte_ptr = (byte_ptr)&hc;
	
	while(in[len] != '\0') {
		hc *= FMV_PRIME_32;
		(*hc_byte_ptr) ^= (byte)in[len];
		++len;
	}

	if(length != nullptr)
		*length = len;
    return hc;
}
}

#endif
