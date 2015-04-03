
#ifndef LIBAXL_VECTOR_ALLOCATOR_GUARD
#define LIBAXL_VECTOR_ALLOCATOR_GUARD

#include "util.h"

namespace libaxl {
struct vector_allocator {
	virtual ~vector_allocator() = default;

	virtual double* alloc(int count) = 0;
	
	virtual void reset() = 0;

	//
	//  Methods for stack allocators
	//
	virtual int push() = 0;
	virtual void pop(int handle) = 0;

	virtual int used() = 0;
	virtual int remaining() = 0;
};

struct alloc_scope {
private:
	vector_allocator* allocator;
	int handle;
public:
	explicit alloc_scope(vector_allocator* allocator) : allocator(allocator) {
		handle = allocator->push();
	}
	~alloc_scope() {
		allocator->pop(handle);
	}

	//Make non-copyable

	alloc_scope(const alloc_scope&) = delete;
	alloc_scope(alloc_scope&&) = delete;

	alloc_scope& operator=(const alloc_scope&) = delete;
	alloc_scope& operator=(alloc_scope&&) = delete;
};
}

#endif
