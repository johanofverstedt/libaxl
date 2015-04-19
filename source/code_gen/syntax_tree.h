
#ifndef LIBAXL_SYNTAX_TREE_GUARD
#define LIBAXL_SYNTAX_TREE_GUARD

#include <cassert>
#include "../util.h"
#include "../arena.h"

namespace libaxl {
using tree_index = int;

struct node {
	int id;
	int param[3];
};

inline
node make_node(int id, int param1 = 0, int param2 = 0, int param3 = 0) {
	node result;

	result.id = id;
	result.param[0] = param1;
	result.param[1] = param2;
	result.param[2] = param3;

	return result;
}

struct syntax_tree {
	node* nodes;
	tree_index used;
	tree_index capacity;
};

inline
syntax_tree make_syntax_tree(arena* arena, tree_index capacity) {
	syntax_tree result;

	result.nodes = allocate<node>(arena, capacity);
	result.used = 0;
	result.capacity = capacity;

	return result;
}

inline
tree_index length(syntax_tree* t) {
	return t->used;
}

inline
tree_index free_capacity(syntax_tree* t) {
	return t->capacity - t->used;
}

inline
tree_index node_index(syntax_tree* t, node* n) {
	tree_index result;

	result = n - t->memory;
}

inline
node* node_from_index(syntax_tree *t, tree_index index) {
	return t->nodes[index];
}

inline
node* append(syntax_tree* t, int id) {
	assert (free_capacity(*t) > 0);

	tree_index insert_index = t->used;

	node* cur = (syntax_node*)(t->nodes + insert_index);
	*cur = make_node(id);
	t->used = insert_index + 1;

	return &t->nodes[insert_index];
}

inline
tree_index append(syntax_tree* t, node* n) {
	assert (free_capacity(*t) > 0);

	tree_index insert_index = t->used;

	t->nodes[insert_index] = *n;
	t->used = insert_index + 1;

	return insert_index;
}


}

#endif
