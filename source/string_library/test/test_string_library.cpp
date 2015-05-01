
#include "../string_library.h"

using namespace string_library;

void fill_table(string_table* st, string_buffer* sb) {
	u32 u8_index = add_string(st, MAKE_STRING_FROM_LITERAL("u8"));
	u32 i8_index = add_string(st, MAKE_STRING_FROM_LITERAL("i8"));
	u32 u32_index = add_string(st, MAKE_STRING_FROM_LITERAL("u32"));
	u32 u64_index = add_string(st, MAKE_STRING_FROM_LITERAL("u64"));
	u32 take_index = add_string(st, MAKE_STRING_FROM_LITERAL("take"));
	u32 take_index2 = add_string(st, MAKE_STRING_FROM_LITERAL("take"));
	u32 u32_index2 = add_string(st, MAKE_STRING_FROM_LITERAL("u32"));

	append(sb, MAKE_STRING_FROM_LITERAL("u8 index: "));
	append(sb, u8_index);
	append_line(sb, make_empty_string());
	append(sb, MAKE_STRING_FROM_LITERAL("u64 index: "));
	append(sb, u64_index);
	append_line_indent(sb, MAKE_STRING_FROM_LITERAL(""), 4);

	print(to_string(sb));

	print(get_string(st, take_index));
}

int main(int argc, char** argv) {
	string_table st = make_string_table(1024 * 1024, 256 * 1024);
	string_buffer sb = make_stack((byte_ptr)malloc(1024 * 1024), 1024 * 1024);

	fill_table(&st, &sb);

	free(sb.ptr);
	free_string_table(&st);

	system("pause");

	return 0;
}