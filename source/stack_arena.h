
#ifndef LIBAXL_STACK_ARENA_GUARD
#define LIBAXL_STACK_ARENA_GUARD

#include "util.h"
#include "arena.h"

namespace libaxl {

template <int SIZE>
class fixed_stack_arena : public stack_arena {
private:
	size_type used_;
	unsigned char memory_[SIZE];
public:
	fixed_stack_arena() : used_(0) {}
	~fixed_stack_arena() = default;
	//Prevent copy construction
	fixed_stack_arena(const fixed_stack_arena&) = delete;
	//Prevent move construction
	fixed_stack_arena(fixed_stack_arena&& that) = delete;

	//Prevent copy assignment
	fixed_stack_arena& operator=(const fixed_stack_arena&) = delete;
	//Prevent move assignment
	fixed_stack_arena& operator=(fixed_stack_arena&&) = delete;

	virtual unsigned char* alloc(size_type count, size_type alignment) override {
		assert(alignment >= 1);

		size_type alignment_error = detail::ptr_alignment_offset(memory_ + used_, alignment);
		size_type adjusted_used = used_ + alignment_error;
		size_type new_used = adjusted_used + count;

		assert(new_used <= SIZE);

		unsigned char* result = memory_ + adjusted_used;
		used_ = new_used;

		return result;
	}

	virtual void reset() override {
		used_ = 0U;
	}

	virtual size_type push() override {
		return used_;
	}

	virtual void pop(size_type handle) override {
		assert(used_ >= handle);
		
		used_ = handle;
	}

	virtual size_type used() override { return used_; }
	virtual size_type capacity() override { return SIZE; }
};

class dynamic_stack_arena : public stack_arena {
private:	
	size_type used_;
	size_type size_;
	unsigned char *memory_;
public:
	explicit dynamic_stack_arena(size_type size) : used_(0U), size_(size) {
		assert(size > 0);

		memory_ = new unsigned char[size];
	}
	~dynamic_stack_arena() {
		if(memory_)
			delete[] memory_;
	}
	//Prevent copy construction
	dynamic_stack_arena(const dynamic_stack_arena&) = delete;
	//Allow move construction
	dynamic_stack_arena(dynamic_stack_arena&& that)
	: used_(that.used_), size_(that.size_), memory_(that.memory_) {
		that.used_ = 0;
		that.size_ = 0;
		that.memory_ = nullptr;
	}

	//Prevent copy assignment
	dynamic_stack_arena& operator=(const dynamic_stack_arena&) = delete;
	//Allow move assignment
	dynamic_stack_arena& operator=(dynamic_stack_arena&& that) {
		used_ = that.used_;
		size_ = that.size_;
		memory_ = that.memory_;

		that.used_ = 0;
		that.size_ = 0;
		that.memory_ = nullptr;

		return *this;
	}

	virtual unsigned char* alloc(size_type count, size_type alignment) override {
		assert(alignment >= 1);

		size_type alignment_error = detail::ptr_alignment_offset(memory_ + used_, alignment);
		size_type adjusted_used = used_ + alignment_error;
		size_type new_used = adjusted_used + count;

		assert(new_used <= size_);

		unsigned char* result = memory_ + adjusted_used;
		used_ = new_used;

		return result;
	}

	virtual void reset() override {
		used_ = 0;
	}

	virtual size_type push() override {
		return used_;
	}

	virtual void pop(size_type handle) override {
		assert(used_ >= handle);

		used_ = handle;
	}

	virtual size_type used() override { return used_; }
	virtual size_type capacity() override { return size_; }
};
}

// LIBAXL_STACK_ARENA_GUARD
#endif
