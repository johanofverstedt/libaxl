
#ifndef STRING_LIBRARY_STACK_GUARD
#define STRING_LIBRARY_STACK_GUARD

#include <assert.h>
#include <string.h>

#include "types.h"

namespace string_library {
struct stack {
	byte_ptr ptr;
	u32 top;
	u32 capacity;
};

inline
stack make_stack(byte_ptr ptr, u32 capacity) {
	stack result;

	result.ptr = ptr;
	result.top = 0U;
	result.capacity = capacity;

	return result;
}

inline
u32 push(stack* s) {
	u32 result;

	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(s->top <= s->capacity);

	result = s->top;

	return result;
}

inline
u32 push_zeros(stack* s, u32 count) {
	u32 result;

	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(s->top + count <= s->capacity);

	result = s->top;

	memset(s->ptr + result, 0, count);

	s->top = result + count;

	return result;
}

template <typename T>
inline
u32 push(stack* s, T* ptr, u32 count) {
	u32 result;

	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(ptr != nullptr);
	assert(s->top + sizeof(T) * count <= s->capacity);

	result = s->top;

	memcpy(s->ptr + result, ptr, count * sizeof(T));

	s->top = result + count;

	return result;
}

inline
void pop(stack* s, u32 top) {
	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(s->top >= top);

	s->top = top;
}

inline
void pop_and_clear(stack* s, u32 top) {
	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(s->top >= top);

	memset(s->ptr + top, 0, s->top - top);

	s->top = top;
}

template <typename T>
inline
void read(stack* s, u32 index, T* out) {
	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(out != nullptr);
	assert(s->top >= index);
	assert(s->top >= index + sizeof(T));

	memcpy(out, s->ptr + index, sizeof(T));
}

template <typename T>
inline
void read(stack* s, u32 index, T* out, u32 count) {
	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(out != nullptr);
	assert(s->top >= index);
	assert(s->top >= index + sizeof(T) * count);

	memcpy(out, s->ptr + index, sizeof(T) * count);
}

inline
byte_ptr resize_stack(stack* s, byte_ptr new_buffer, u32 new_capacity) {
	// preconditions
	assert(s != nullptr);
	assert(s->ptr != nullptr);
	assert(new_buffer != nullptr);
	assert(s->used <= new_capacity);

	byte_ptr old_ptr = s->ptr;
	if(old_ptr != new_buffer) { // support for realloc
		memcpy(new_buffer, old_ptr, s->used);
	}

	s->capcity = new_capacity;

	return old_ptr;
}
}

#endif
