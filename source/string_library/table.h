
#ifndef STRING_LIBRARY_STRING_TABLE_GUARD
#define STRING_LIBRARY_STRING_TABLE_GUARD

#include "str_type.h"
#include "stack.h"

namespace string_library {
// suggested in:
// http://www.orcca.on.ca/~yxie/courses/cs2210b-2011/htmls/extra/PlanetMath_%20goodhashtable.pdf
u32 HASH_TABLE_SIZES[] = {
	53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613,
	393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
	201326611, 402653189, 805306457, 1610612741
};

struct table_entry {
	u32 index;
	u32 hash;
};

struct string_table {
	stack str_buf;

	// associative index

	table_entry* entries;
	u32 entry_count;
};

inline
str make_string_ref_from_string_table(cstring buffer, u32 used, u32 index) {
	str result;
	cstring indexed_buffer = (buffer + index);

	str_info info;
	memcpy(&info, indexed_buffer - (u32)sizeof(string_info), sizeof(string_info));

	result = make_string(indexed_buffer, info);

	return result;
}

inline
u32 next_suitable_size(u32 requested_size) {
	u32 len = sizeof(table_sizes) / sizeof(table_sizes[0]);
	u32 len_1 = len - 1;

	assert(requested_size <= table_sizes[len_1]);

	for(u32 i = 0; i < len_1; ++i) {
		if(requested_size <= table_sizes[i])
			return table_sizes[i];
	}

	return table_sizes[len_1];
}

inline
string_table make_string_table(u32 string_buffer_length, u32 hash_table_length) {
	string_table result;

	result.str_buf = make_stack(malloc(string_buffer_length), string_buffer_length);

	result.entry_count = next_suitable_size(hash_table_length);
	result.entries = (table_entry*)malloc(result.entry_count * sizeof(table_entry));

	memset(result.entries, 0, result.entry_count * sizeof(table_entry));

	return result;
}

inline
void free_string_table(string_table* t) {
	u32 entry_count = t->entry_count;

	free(t->str_buf.ptr);
	free(t->entries);

	t->str_buf.ptr = nullptr;
	t->entries = nullptr;

	t->top = 0;
	t->capacity = 0;
	t->entries = 0;
	t->entry_count = 0;
}

inline
u32 push_string(string_table* t, str s) {
	u32 result;

	push(&t->str_buf, &s.info, sizeof(str_info));
	
	cstring src = string_to_cstring(s);
	result = push(&t->str_buf, src, length(s));

	return result;
}

inline
str get_string(string_table* t, u32 index) {
	str result;

	assert(index >= sizeof(str_info));

	byte_ptr src = t->str_buf.ptr + index;

	result.ptr = t->str_buf.ptr + index;
	memcpy(&result.info, src - sizeof(str_info), sizeof(str_info));

	return result;
}

inline
u32 check_or_add_at_index(table_entry* entry, stack* haystack, str needle, u32 hash_value) {
	u32 result = 0U; //default to no match

	u32 entry_index = entry->index;
	u32 entry_hash = entry->hash;

	if(entry_index != 0U) {
		// compare contents
		if(hash_value == entry_hash) {
			str_info info;

			byte_ptr ptr = stack_ptr(haystack, entry_index); 
			memcpy(&info, ptr - sizeof(str_info), sizeof(str_info));
			
			if(info.length == length(s)) {
				if(memcmp(ptr, string_to_cstring(s), info.length) == 0)
					result = entry_index; //the needle was found
			}
		}
	} else {
		// empty table entry... insert string
		push(haystack, &needle.info, sizeof(str_info));
	
		cstring src = string_to_cstring(needle);
		result = push(haystack, src, length(s));
	}

	return result;
}

inline
u32 add_string(string_table* t, str s) {
	u32 result = 0U;

	stack* str_buf = t->str_buf;
	u32 entry_count = t->entry_count;
	table_entry* entry_array = t->entries;
	u32 hash_value = hash(s);
	u32 hash_index = hash_value % entry_count;

	// search the first part of the table, from the hash_index and forward
	for(u32 i = hash_index; i < entry_count && result == 0U; ++i) {
		result = check_or_add_at_index(entry_array + i, str_buf, s, hash_value);
	}
	// search the second part of the table, after wrapping around to index 0
	for(u32 i = 0; i < hash_index && result == 0U; ++i) {
		result = check_or_add_at_index(entry_array + i, str_buf, s, hash_value);
	}

	return result;
}
}

#endif
