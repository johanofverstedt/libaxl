
#ifndef LIBAXL_SYNTAX_TREE_GUARD
#define LIBAXL_SYNTAX_TREE_GUARD

#include <cassert>
#include "../util.h"
#include "../arena.h"

namespace libaxl {
using tree_index = int;

struct syntax_node {
	int id;
	int children;
	tree_index size;
	tree_index sub_tree_size;
};

inline
syntax_node make_syntax_node(int id) {
	syntax_node result;

	result.id = id;
	result.children = 0;
	result.size = sizeof(syntax_node);
	result.sub_tree_size = 0;

	return result;
}

struct syntax_tree_iterator {
	syntax_tree* tree;
	tree_index index;
	tree_index level;
	tree_index stack[256];
};

struct syntax_tree {
	unsigned char* memory;
	tree_index used;
	tree_index capacity;
};

inline
tree_index length(syntax_tree t) {
	return t.used;
}

inline
tree_index free_capacity(syntax_tree t) {
	return t.capacity - t.used;
}

inline
tree_index align_index(unsigned char* ptr, tree_index index, size_t alignment) {
	assert(alignment & (alignment - 1) == 0); // require alignment to be a power of 2

	size_t a = ((size_t)ptr) + (size_t)index;
	size_t r = a & (alignment - 1);

	if(r == 0)
		return index;
	return index + (tree_index)(alignment - r);
}

inline
tree_index append(syntax_tree* t, int id) {
	assert (free_capacity(*t) >= sizeof(syntax_node));

	tree_index insert_index = align_index(t->memory, t->used, 8U);

	syntax_node* cur = (syntax_node*)(t->memory + insert_index);

	*cur = make_syntax_node(id);

	t->used = insert_index + sizeof(syntax_node);

	return old_used;
}

template <typename T>
T* get_tree_node(syntax_tree *t, tree_index index) {
	index = align_index(t->memory, t->used, 8U);

	return (T*)(t->memory + index);
}

struct syntax_tree_writer {

};
}

#endif
