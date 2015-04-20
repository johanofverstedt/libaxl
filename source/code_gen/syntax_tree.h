
#ifndef LIBAXL_SYNTAX_TREE_GUARD
#define LIBAXL_SYNTAX_TREE_GUARD

#include <cassert>
#include "../util.h"
#include "../arena.h"

namespace libaxl {
struct syntax_tree {
	u64* memory;
	u64 used;
	u64 capacity;
};

inline
syntax_tree make_syntax_tree(arena* arena, tree_index capacity) {
	syntax_tree result;

	result.memory = allocate<u64>(arena, capacity);
	result.used = 0U;
	result.capacity = capacity;

	return result;
}

inline
u64 length(syntax_tree* t) {
	return t->used;
}

inline
u64 free_capacity(syntax_tree* t) {
	return t->capacity - t->used;
}

inline
u64 push(syntax_tree* t, u64 value) {
	assert (free_capacity(*t) > 0);

	tree_index insert_index = t->used;

	t->memory[insert_index] = value;
	t->used = insert_index + 1;

	return insert_index;
}
}

#endif
