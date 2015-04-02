
#ifndef ALLOCATOR_INTERFACE_GUARD
#define ALLOCATOR_INTERFACE_GUARD

#include "util.h"

namespace libaxl {
struct vector_allocator {
	virtual ~vector_allocator() {}
	virtual double* alloc(int count) = 0;
};
}

#endif
