
#ifndef LIBAXL_VECTOR_ALLOCATOR_GUARD
#define LIBAXL_VECTOR_ALLOCATOR_GUARD

#include "util.h"

namespace libaxl {
struct vector_allocator {
	virtual ~vector_allocator() {}
	virtual double* alloc(int count) = 0;
	
	//
	//  Methods for stack allocators
	//
	virtual int push() = 0;
	virtual void pop(int handle) = 0;
};
}

#endif
