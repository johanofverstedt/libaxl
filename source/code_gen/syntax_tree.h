
#ifndef LIBAXL_SYNTAX_TREE_GUARD
#define LIBAXL_SYNTAX_TREE_GUARD

#include <cassert>
#include "../util.h"
#include "../arena.h"

#define LIBAXL_LAST_BIT_COUNT(T) (sizeof(syntax_tree_value) * 8 - 1)

namespace libaxl {
using syntax_tree_index = u64;
using syntax_tree_value = u32;

struct syntax_tree {
	syntax_tree_value* memory;
	syntax_tree_index used;
	syntax_tree_index capacity;
};

inline
syntax_tree make_syntax_tree(arena* arena, syntax_tree_index capacity) {
	syntax_tree result;

	assert(arena != nullptr);

	result.memory = allocate<syntax_tree_value>(arena, capacity);
	result.used = syntax_tree_index(0);
	result.capacity = capacity;

	return result;
}

inline
syntax_tree_index length(syntax_tree t) {
	assert(t != nullptr);
	assert(t->capacity >= t->used);

	return t->used;
}

inline
syntax_tree_index free_capacity(syntax_tree* t) {
	assert(t != nullptr);
	assert(t->capacity >= t->used);

	return t->capacity - t->used;
}

inline
bool is_head(syntax_tree* t, syntax_tree_index index) {
	assert(t != nullptr);
	assert(index < length(t));

	return (t->memory[index] >> LIBAXL_LAST_BIT_COUNT(syntax_tree_value)) == 0;
}

inline
bool is_tail(syntax_tree* t, syntax_tree_index index) {
	return !is_head(t, index);
}

inline
syntax_tree_value get_head(syntax_tree* t, syntax_tree_index index) {
	syntax_tree_value result;

	assert(t != nullptr);
	assert(index < length(t));

	result = t->memory[index];

	assert(is_head(t, index));

	return result;
}

inline
syntax_tree_value get_tail(syntax_tree* t, syntax_tree_index index) {
	syntax_tree_value result;

	assert(t != nullptr);
	assert(index < length(t));

	result = t->memory[index];

	assert(is_head(t, index));

	return result;	
}

inline
syntax_tree_index push(syntax_tree* t, syntax_tree_value value) {
	assert(free_capacity(*t) > 0);

	syntax_tree_index insert_index = t->used;

	t->memory[insert_index] = value;
	t->used = insert_index + 1;

	return insert_index;
}

inline
syntax_tree_index push_head(syntax_tree* t, syntax_tree_value value) {
	syntax_tree_index result;

	assert((value >> LIBAXL_LAST_BIT_COUNT(syntax_tree_value)) == 0);

	result = push(t, value);

	return result;
}

inline
syntax_tree_index push_tail(syntax_tree* t, syntax_tree_value value) {
	syntax_tree_index result;
	value |= (1 << LIBAXL_LAST_BIT_COUNT(syntax_tree_value));

	result = push(t, value);

	return result;
}

inline
syntax_tree_index find_head(syntax_tree* t, syntax_tree_index index) {
	syntax_tree_index end = t->used;

	while(index < head && is_tail(t, index)) {
		++index;
	}

	return index;
}
}

#endif
