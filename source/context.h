
#ifndef LIBAXL_CONTEXT_GUARD
#define LIBAXL_CONTEXT_GUARD

#include "util.h"
#include "arena.h"

namespace libaxl {
struct context {
	arena* arena;
	const char* id;
};
inline
context make_context(arena *arena, const char* id) {
	context result;

	result.arena = arena;
	result.id = id;

	return result;
}
inline
context make_context(arena *arena) {
	return make_context(arena, nullptr);
}

inline
arena* get_context_arena(context* context) {
	assert(context != nullptr);
	return context->arena;
}
inline
void set_context_arena(context* context, arena* arena) {
	assert(context != nullptr);
	context->arena = arena;
}

class context_scope {
private:
	context c;
	context* ptr;
public:
	context_scope(context* context_ptr) : ptr(context_ptr) {
		assert(context_ptr != nullptr);
		c = *context_ptr;
	}
	~context_scope() {
		assert(ptr != nullptr);
		*ptr = c;
	}
	context_scope(const context_scope&) = delete;
	context_scope(context_scope&&) = delete;

	void operator=(const context_scope&) = delete;
	void operator=(context_scope&&) = delete;
};
}

#endif
