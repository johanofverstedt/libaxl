
#ifndef LIBAXL_STACK_ARENA_GUARD
#define LIBAXL_STACK_ARENA_GUARD

#include "util.h"

namespace libaxl {
template <int SIZE>
struct stack_vector_arena : vector_arena {
	int used_;
	double memory_[SIZE];

	stack_vector_arena() : used_(0) {}
	~stack_vector_arena() = default;
	//Prevent copy construction
	stack_vector_arena(const stack_vector_arena&) = delete;
	//Prevent move construction
	stack_vector_arena(stack_vector_arena&& that) = delete;

	//Prevent copy assignment
	stack_vector_arena& operator=(const stack_vector_arena&) = delete;
	//Prevent move assignment
	stack_vector_arena& operator=(stack_vector_arena&&) = delete;

	virtual double* alloc(int count) override {
		assert(count >= 0);
		assert(used_ + count <= SIZE);

		double* result = memory_ + used_;
		used_ += count;

		return result;
	}

	virtual void reset() override {
		used_ = 0;
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

struct dynamic_stack_vector_arena : vector_arena {
	int used_;
	int size_;
	double *memory_;

	explicit dynamic_stack_vector_arena(int size) : used_(0), size_(size) {
		assert(size > 0);

		memory_ = new double[size];
	}
	~dynamic_stack_vector_arena() {
		if(memory_)
			delete[] memory_;
	}
	//Prevent copy construction
	dynamic_stack_vector_arena(const dynamic_stack_vector_arena&) = delete;
	//Allow move construction
	dynamic_stack_vector_arena(dynamic_stack_vector_arena&& that)
	: used_(that.used_), size_(that.size_), memory_(that.memory_) {
		that.used_ = 0;
		that.size_ = 0;
		that.memory_ = nullptr;
	}

	//Prevent copy assignment
	dynamic_stack_vector_arena& operator=(const dynamic_stack_vector_arena&) = delete;
	//Allow move assignment
	dynamic_stack_vector_arena& operator=(dynamic_stack_vector_arena&& that) {
		used_ = that.used_;
		size_ = that.size_;
		memory_ = that.memory_;

		that.used_ = 0;
		that.size_ = 0;
		that.memory_ = nullptr;

		return *this;
	}

	virtual double* alloc(int count) override {
		assert(count >= 0);
		assert(used_ + count <= size_);

		double* result = memory_ + used_;
		used_ += count;

		return result;
	}

	virtual void reset() override {
		used_ = 0;
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

// LIBAXL_STACK_ARENA_GUARD
#endif
