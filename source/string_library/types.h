
#ifndef STRING_LIBRARY_TYPES_GUARD
#define STRING_LIBRARY_TYPES_GUARD

#include <stdint.h>

namespace string_library {
// basic zero-terminated c-string type definitions
typedef char*       mutable_cstring;
typedef const char* cstring;

typedef unsigned char  byte;
typedef unsigned char* byte_ptr;

// integer type definitions
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;
}

#endif
