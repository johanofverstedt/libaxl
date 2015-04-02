
#ifndef LIBAXL_STACK_ALLOCATOR_GUARD
#define LIBAXL_STACK_ALLOCATOR_GUARD

#include "util.h"

namespace libaxl {
template <int size>
struct stack_vector_allocator : vector_allocator {
	int used;
	double memory[size];

	virtual double* alloc(int count) override {
		assert(count >= 0);
		assert(used + count <= size);

		double* result = memory + used;
		used += count;

		return result;
	}

	virtual int push() override {
		return used;
	}

	virtual void pop(int handle) override {
		assert(used >= handle);
		
		used = handle;
	}
};

struct dynamic_stack_vector_allocator : vector_allocator {
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

	virtual double* alloc(int count) override {
		assert(count >= 0);
		assert(used + count <= size);

		double* result = memory + used;
		used += count;

		return result;
	}

	virtual int push() override {
		return used;
	}

	virtual void pop(int handle) override {
		assert(used >= handle);
		
		used = handle;
	}	
};
}

#endif
