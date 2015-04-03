
#ifndef LIBAXL_VECTOR_arena_GUARD
#define LIBAXL_VECTOR_arena_GUARD

#include "util.h"

namespace libaxl {
	
struct vector_arena {
	virtual ~vector_arena() = default;

	virtual double* alloc(int count) = 0;
	
	virtual void reset() = 0;

	//
	//  Methods for stack arenas
	//
	virtual int push() = 0;
	virtual void pop(int handle) = 0;

	virtual int used() = 0;
	virtual int remaining() = 0;
};

struct alloc_scope {
private:
	vector_arena* arena;
	int handle;
public:
	explicit alloc_scope(vector_arena* arena) : arena(arena) {
		assert(arena != nullptr);
		handle = arena->push();
	}
	~alloc_scope() {
		assert(arena != nullptr);
		arena->pop(handle);
	}

	//Make non-copyable/non-movable

	alloc_scope(const alloc_scope&) = delete;
	alloc_scope(alloc_scope&&) = delete;

	alloc_scope& operator=(const alloc_scope&) = delete;
	alloc_scope& operator=(alloc_scope&&) = delete;
};
}

#endif
