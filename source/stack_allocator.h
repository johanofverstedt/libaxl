
#ifndef LIBAXL_STACK_ALLOCATOR_GUARD
#define LIBAXL_STACK_ALLOCATOR_GUARD

#include "util.h"

namespace libaxl {
template <int SIZE>
struct stack_vector_allocator : vector_allocator {
	int used_;
	double memory_[SIZE];

	stack_vector_allocator() : used_(0) {}

	virtual double* alloc(int count) override {
		assert(count >= 0);
		assert(used_ + count <= SIZE);

		double* result = memory_ + used_;
		used_ += count;

		return result;
	}

	virtual int push() override {
		return used_;
	}

	virtual void pop(int handle) override {
		assert(used_ >= handle);
		
		used_ = handle;
	}

	virtual int used() override { return used_; }
	virtual int remaining() override { return SIZE - used_; }
};

struct dynamic_stack_vector_allocator : vector_allocator {
	int used_;
	int size_;
	double *memory_;

	explicit dynamic_stack_vector_allocator(int size) : used_(0), size_(size) {
		assert(size > 0);

		memory_ = new double[size];
	}

	~dynamic_stack_vector_allocator() {
		delete[] memory_;
	}

	virtual double* alloc(int count) override {
		assert(count >= 0);
		assert(used_ + count <= size_);

		double* result = memory_ + used_;
		used_ += count;

		return result;
	}

	virtual int push() override {
		return used_;
	}

	virtual void pop(int handle) override {
		assert(used_ >= handle);

		used_ = handle;
	}	

	virtual int used() override { return used_; }
	virtual int remaining() override { return size_ - used_; }
};
}

#endif
