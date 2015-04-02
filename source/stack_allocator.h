
#ifndef LIBAXL_STACK_ALLOCATOR_GUARD
#define LIBAXL_STACK_ALLOCATOR_GUARD

#include "util.h"

namespace libaxl {
template <int size>
struct stack_vector_allocator {
	int used;
	double memory[size];

	double* alloc(int count) {
		assert(count >= 0);
		assert(used + count <= size);

		double* result = memory + used;
		used += count;

		return result;
	}
};

struct dynamic_stack_vector_allocator {
	int used;
	int size;
	double *memory;

	expicit dynamic_stack_vector_allocator(int size) : used(0), size(size) {
		assert(size > 0);

		memory = new double[size];
	}

	~dynamic_stack_vector_allocator() {
		delete[] memory;
	}

	double* alloc(int count) {
		assert(count >= 0);
		assert(used + count <= size);

		double* result = memory + used;
		used += count;

		return result;
	}
};

template <typename T>
inline
double* allocate(T& allocator, int count) {
	return allocator.alloc(count);
}

template <typename T>
inline
int alloc_push(T& allocator) {
	return allocator.used;
}

template <typename T>
inline
void alloc_pop(T& allocator, int push_index) {
	allocator.used = push_index;
}
}

#endif
