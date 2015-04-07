
#include "../stack_arena.h"
#include "../strings.h"
#include "../string_buffer.h"
#include <iostream>

#include "../codegen/function_declaration.h"

void print(libaxl::const_string s, bool newline = true) {
	char ss[512];
	memcpy(ss, s.array, length(s));
	ss[length(s)] = '\0';
	
	std::cout << ss;
	if (newline)
		std::cout << std::endl;
}

int main(int argc, char** argv) {
	using namespace libaxl;

	dynamic_stack_arena arena{ new unsigned char[1024], 1024 };

	function_declaration f1;

	f1 = make_function_declaration(&arena,
		wrap_cstring(&arena, "minimum"),
		wrap_cstring(&arena, "double"),
		2,
		function_inline);

	set_parameter_name(f1, 0, wrap_cstring(&arena, "x"));
	set_parameter_name(f1, 1, wrap_cstring(&arena, "y"));
	set_parameter_type(f1, 0, wrap_cstring(&arena, "double"));
	set_parameter_type(f1, 1, wrap_cstring(&arena, "double"));

	code_gen cg;
	cg.string_buffer = make_string_buffer(&arena, 512);
	cg.indent = 0;
	codegen(cg, f1, true);

	print(to_string(cg.string_buffer));

	std::cout << "strlen(hej) == " << strlen("hej");

	int in;
	std::cin >> in;
}
