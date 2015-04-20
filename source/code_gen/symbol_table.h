
#ifndef LIBAXL_SYMBOL_TABLE_GUARD
#define LIBAXL_SYMBOL_TABLE_GUARD

#include "../arena.h"
#include "../util.h"

#include "string.h"

struct symbol {
	const char* name;
	size_t length;
};

inline
bool operator==(symbol s1, symbol s2) {
	assert(s1.name != nullptr && s2.name != nullptr);
	assert(s1.length > 0 && s2.length > 0);

	if(s1.length != s2.length)
		return false;

	for(size_t i = 0; i < s1.length; ++i) {
		if(s1.name[i] != s2.name[i])
			return false;
	}

	return true;
}

inline
bool operator!=(symbol s1, symbol s2) {
	return !(s1 == s2);
}

struct symbol_table {
	symbol* elem;
	int size;
	int used;
};

struct symbol_search_result {
	int index;
	bool was_found;
};

inline
symbol_table make_symbol_table(arena* arena, int size) {
	symbol_table result;

	result.elem = allocate<symbol>(arena, size);
	result.size = size;

	memset(result.elem, 0, sizeof(symbol) * size);

	return result;
}

//Replace this horrid hash generator with one that is
//actually any good.
inline
size_t symbol_hash(const char* s, size_t slen) {
	size_t v = 0;

	for(size_t i = 0; i < slen; ++i) {
		v = (v << 3) ^ (v * 13);
		size_t c = (size_t)s[i];
		v += c;
	}

	return v;
}

inline
size_t symbol_hash(const char* s) {
	int result;
	size_t slen = strlen(s);

	result = symbol_hash(s, slen);

	return result;
}

inline
symbol get_symbol(symbol_table* t, int index) {
	assert(index >= 0 && index < t->used);
	assert(t->elem[index].name != nullptr);

	return t->elem[index];
}

inline
symbol_search_result find_symbol(symbol_table* t, symbol s) {
	symbol_search_result result;

	result.index = -1;
	result.was_found = false;

	int t_size = t->size;
	symbol* elem = t->elem;

	size_t raw_hash = symbol_hash(s, slen) % (size_t)t_size;
	int hash = (int)raw_hash;

	for(int i = hash; i < t_size; ++i) {
		if(elem[i].name == nullptr) {
			result.index = i;
			return result;
		}

		if(elem[i] == s) {
			result.index = i;
			result.was_found = true;
			return result;
		}
	}

	for(int i = 0; i < hash; ++i) {
		if(elem[i].name == nullptr) {
			result.index = i;
			return result;
		}

		if(elem[i] == s) {
			result.index = i;
			result.was_found = true;
			return result;
		}
	}

	return result;
}

inline
symbol_search_result find_symbol(symbol_table* t, const char* s) {
	symbol_search_result result;

	size_t slen = strlen(s);

	symbol new_symbol;

	new_symbol.name = s;
	new_symbol.length = slen;

	result = find_symbol(t, new_symbol);

	return result;
}

inline
int add_symbol(symbol_table* t, symbol s) {
	int result;

	int t_size = t->size;
	symbol* elem = t->elem;

	symbol_search_result search_result = find_symbol(s);

	if(search_result.was_found) {
		result = search_result.index;
	} else {
		assert(search_result.index >= 0 && search_result.index < t_size);

		elem[search_result.index] = s;
		result = search_result.index;
	}

	return result;
}

inline
int add_symbol(symbol_table* t, const char* s) {
	int result;

	size_t slen = strlen(s);

	symbol new_symbol;

	new_symbol.name = s;
	new_symbol.length = slen;

	result = add_symbol(t, new_symbol);

	return result;
}

#endif
