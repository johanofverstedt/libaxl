
#ifndef LIBAXL_ARENA_GUARD
#define LIBAXL_ARENA_GUARD

#include "util.h"

namespace libaxl {

struct arena {
	virtual ~arena() = default;

	virtual unsigned char* alloc(size_type count, size_type alignment) = 0;
	
	virtual void reset() = 0;
};

struct stack_arena : arena {
	virtual ~stack_arena() = default;

	virtual size_type push() = 0;
	virtual void pop(size_type handle) = 0;

	virtual size_type used() = 0;
	virtual size_type capacity() = 0;
	virtual size_type remaining() {
		return capacity() - used();
	}
};

template <typename T>
inline
T* allocate(arena* arena, index_type count) {
	assert(arena != nullptr);
	assert(count >= 0);

	size_type sz = (size_type)sizeof(T);
	auto alignment = detail::adjust_alignment(sz);
	auto result = (T*)arena->alloc((size_type)count * sz(T), alignment);

	return result;
}

template <>
inline
double* allocate<double>(arena* arena, index_type count) {
	assert(arena != nullptr);
	assert(count >= 0);

	size_type sz = (size_type)sizeof(double);
	size_type alignment = 16U;
	auto result = (double*)arena->alloc((size_type)count * sz, alignment);

	return result;
}

template <>
inline
float* allocate<float>(arena* arena, index_type count) {
	assert(arena != nullptr);
	assert(count >= 0);

	size_type sz = (size_type)sizeof(float);
	size_type alignment = 16U;
	auto result = (float*)arena->alloc((size_type)count * sz, alignment);

	return result;
}

struct stack_arena_scope {
private:
	stack_arena* arena;
	size_type handle;
public:
	explicit stack_arena_scope(stack_arena* arena) : arena(arena) {
		assert(arena != nullptr);
		handle = arena->push();
	}
	~stack_arena_scope() {
		assert(arena != nullptr);
		arena->pop(handle);
	}

	//Make non-copyable/non-movable

	stack_arena_scope(const stack_arena_scope&) = delete;
	stack_arena_scope(stack_arena_scope&&) = delete;

	stack_arena_scope& operator=(const stack_arena_scope&) = delete;
	stack_arena_scope& operator=(stack_arena_scope&&) = delete;
};
}

#endif
