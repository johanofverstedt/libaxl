
#ifndef STRING_TABLE_GUARD
#define STRING_TABLE_GUARD

#include "../util.h"
#include "../arena.h"

#define FMV_OFFSET 14695981039346656037ULL
#define FMV_PRIME 1099511628211ULL

namespace libaxl {

u32 table_sizes[] = {
	53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613,
	393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
	201326611, 402653189, 805306457, 1610612741
};

struct table_entry {
	table_entry* next;
	u32 start_index;
	u32 hash;
};

struct string_table {
	char* str;
	u32 used;
	u32 capacity;

	// associative index

	table_entry* entries;
	u32 entry_count;
};

inline
u32 next_suitable_size(u32 requested_size) {
	u32 len = sizeof(table_sizes) / sizeof(table_sizes[0]);
	u32 len_1 = len - 1;

	for(u32 i = 0; i < len_1; ++i) {
		if(requested_size <= table_sizes[i])
			return table_sizes[i];
	}

	return table_sizes[len_1];
}

inline
string_table make_string_table(u32 string_buffer_length, u32 hash_table_length) {
	string_table result;

	result.str = (char*)malloc(string_buffer_length);
	result.used = 1;
	result.capacity = string_buffer_length;

	result.str[0] = '\0';

	hash_table_length = next_suitable_size(hash_table_length);

	result.entries = (table_entry*)malloc(hash_table_length * sizeof(table_entry));
	result.entry_count = hash_table_length;

	memset(entries, 0, hash_table_length * sizeof(table_entry));

	return result;
}

inline
void free_string_table(string_table* t) {
	u32 entry_count = t->entry_count;

	for(u32 i = 0; i < entry_count; ++i) {
		table_entry* cur = t->entries[i]->next;
		while(cur) {
			table_entry* prev = cur;
			cur = cur->next;
			free(prev);
		}
	}

	free(t->str);
	free(t->entries);

	t->used = 0;
	t->capacity = 0;
	t->entries = 0;
	t->entry_count = 0;
}

inline
u32 hash_from_string(const char* string, u32 string_length) {
	u64 hc = FMV_OFFSET;
	unsigned char* hc_char_ptr = (unsigned char*)&hc;
	for(u32 i = 0; i < string_length; ++i) {
		hc *= FMV_PRIME;
		(*hc_char_ptr) ^= (unsigned char)string[i];
	}
    return (u32)hc;
}

inline
u64 add_string(string_table* t, const char* string, u32 string_length) {
	u32 hash = hash_from_string(string, string_length);
	u32 old_used = t->used;
	u32 outer_index = hash % t->entry_count;
	u32 inner_index = 0U;

	table_entry* cur;
	cur = &t->entries[outer_index];
	while(cur->start_index != 0) {
		if(strcmp(t->str + cur->start_index, string) == 0) {
			return inner_index;
		}

		++inner_index;

		if(cur->next == 0) {
			cur->next = malloc(table_entry * sizeof(table_entry));
			memset(cur->next, 0, sizeof(table_entry));
		}

		cur = cur->next;
	}

	// string was not found. add it to the table
	{
		u32 new_used = old_used + string_length + 1;
		assert(new_used <= t->capacity); //TODO: Handle enlargements

		memcpy(t->str + old_used, string, string_length + 1);
		t->used = new_used;

		cur->start_index = old_used;
	}

	u64 result = ((u64)outer_index << 32) | ((u64)inner_index);
	return result;
}

inline
const char* get_string(string_table* t, u64 index) {
	u32 outer_index = (u32)(index >> 32);
	u32 inner_index = (u32)(index & 4294967295U);

	table_entry* cur = &t->entries[outer_index];
	while(inner_index) {
		assert(cur->next != 0);
		++inner_index;
		cur = cur->next;
	}

	assert(cur->start_index != 0);

	return t->str + cur->start_index;
}

}

#endif
