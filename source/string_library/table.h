
#ifndef STRING_LIBRARY_STRING_TABLE_GUARD
#define STRING_LIBRARY_STRING_TABLE_GUARD

#include "string_ref.h"

namespace string_table {

#define FMV_OFFSET 14695981039346656037ULL
#define FMV_PRIME 1099511628211ULL

namespace libaxl {
// suggested in:
// http://www.orcca.on.ca/~yxie/courses/cs2210b-2011/htmls/extra/PlanetMath_%20goodhashtable.pdf
u32 HASH_TABLE_SIZES[] = {
	53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613,
	393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319,
	201326611, 402653189, 805306457, 1610612741
};

inline
u32 hash_u32_from_string(const char* string, u32 string_length) {
	u64 hc = FMV_OFFSET;
	unsigned char* hc_char_ptr = (unsigned char*)&hc;
	for(u32 i = 0; i < string_length; ++i) {
		hc *= FMV_PRIME;
		(*hc_char_ptr) ^= (unsigned char)string[i];
	}
    return (u32)hc;
}

//
//  string reference type
//  supports the small string optimization
//  i.e. when the string size (including terminating '\0')
//  is <= than the size of ptr, the string is contained
//  in the string_ref object.
//

struct string_info {
	u32 length;
	u32 hash;
};

struct string_ref {
	string_info info;
	union {
		const char* ptr;
		char str[sizeof(const char*)];
	};
};

struct string_table {
	char* str;
	u32 used;
	u32 capacity;

	// associative index

	u32* index_table;
	u32 table_size;
};

inline
string_info make_string_info(const char* str, u32 length) {
	string_info result;

	result.length = length;
	result.hash = hash_u32_from_string(str, length);

	return result;
}

inline
string_info make_string_info(const char* str) {
	string_info result;
	size_t len = strlen(str);

	assert(len <= 4294967295U);

	result = make_string_info(str, (u32)len);

	return result;
}

//
//  function that verifies that a valid string exists at position index
//  in the specified buffer.
//
//  performance warning: this verification function is very expensive
//  and should only be enabled in the slowest debug builds.
//
inline
bool validate_string_at_index(const char* buffer, u32 size, u32 index) {
	bool result = true;

	// verify that the string is not "null" (0 index) or overflows the size of the buffer
	if(index == 0U || index >= size) {
		result = false;
		return result;
	}

	// read the string_info structure from the buffer
	string_info info;
	memcpy(&info, buffer + index - sizeof(string_info), sizeof(string_info));

	// verify that the end of the string does not overflow the size of the buffer
	u64 end_index = (u64)index + (u64)info.length + 1ULL;
	if(end_index >= (u64)size) {
		result = false;
		return result;
	}
	
	// verify that the string hash matches the recorded hash
	u32 hash = hash_u32_from_string(buffer + index, info.length);
	if(hash != info.hash) {
		result = false;
		return result;
	}

	// verify that the string is null-terminated at the correct position
	if(buffer[index + info.length] != '\0') {
		result = false;
		return result;
	}

	return result;
}

inline
string_ref make_string_ref_from_string_table(const char* buffer, u32 used, u32 index) {
	string_ref result;
	const char* str = buffer + index;

	assert(buffer != 0);
	assert(index >= (u32)sizeof(string_info));

	//performance warning: very expensive assert
	assert(validate_string_at_index(buffer, used, index));

	memcpy(&result.info, str - (u32)sizeof(string_info), sizeof(string_info));

	if(result.info.length < sizeof(const char*)) { // embed string in ref
		memcpy(result.str, buffer, result.info.length);
		result.str[result.info.length] = '\0';
	} else {
		result.ptr = buffer + index;
	}

	return result;
}

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
	result.used = 0;
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
u32 push_string(string_table* t, string_ref s) {
	u32 old_used = t->used;

	u32 new_used = old_used + string_length + sizeof(string_info) + 1;
	assert(new_used <= t->capacity); //TODO: Handle enlargements

	char* buf = t->str + old_used;

	memcpy(buf, &s.info, sizeof(string_info));
	buf += sizeof(string_info);
	memcpy(buf, s.ptr, s.info.length);
	buf[s.info.length] = '\0';

	t->used = new_used;

	return old_used + sizeof(string_info);
}

inline
string_ref get_string(string_table* t, u32 index) {
	string_ref result;

	result.ptr = t->str + index;
	memcpy(&result.info, t->str - sizeof(string_info), sizeof(string_info));

	return result;
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
			return cur->start_index;
			//return inner_index;
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

/*

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
*/	
}

#endif
