
#include <stdio.h>
#include <iostream>
#include "vector_code_gen.h"

#include "value.h"
#include "expr.h"
#include "struct.h"
#include "function.h"

using namespace libaxl;
/*
void open_scope(cg_context* context) {
	string_buffer& sb = context->sb;
	
	context->indent += 4;

	append(sb, "{");
	newline(sb);
}

void close_scope(cg_context* context) {
	string_buffer& sb = context->sb;

	int new_indent = context->indent - 4;
	if(new_indent < 0)
		new_indent = 0;

	context->indent = new_indent;
	newline(sb, context->indent);
	append(sb, "}");
	newline(sb, context->indent);
}

void close_scope(cg_context* context, const char* comment) {
	string_buffer& sb = context->sb;

	int new_indent = context->indent - 4;
	if(new_indent < 0)
		new_indent = 0;

	context->indent = new_indent;
	newline(sb, context->indent);
	append(sb, "} // ");
	append(sb, comment);
	newline(sb, context->indent);
}
*/
void write_include_file(cg_context* context, const char* path) {
	string_buffer& sb = context->sb;

	append(sb, "#include \"");
	append(sb, path);
	append(sb, "\"");
	newline(sb);
}

void write_inline(cg_context* context, bool always_inline) {
	string_buffer& sb = context->sb;

	if(always_inline) {
		append(sb, "ALWAYS_INLINE");
	} else {
		append(sb, "inline");
	}
	newline(sb, context->indent);
}

void write_func0_header(cg_context* context, const char* name, const char* return_type) {
	string_buffer& sb = context->sb;

	space(sb, context->indent);
	append(sb, "auto ");
	append(sb, name);
	append(sb, " -> ");
	append(sb, return_type);
	append(sb, " ");

	open_scope(context);
}

void write_sum_func(cg_context* context) {
	append(context->sb, "double sum(array_double v) {");
	indent(context);
	newline(context->sb, context->indent);
	append(context->sb, "double s = 0.0;");
	newline(context->sb, context->indent);
	append(context->sb, "for(i32 i = 0; i < v.count; ++i) {");
	indent(context);
	newline(context->sb, context->indent);
	append(context->sb, "s += v.elem[i];");
	unindent(context);
	newline(context->sb, context->indent);
	append(context->sb, "} //for (i)");
	newline(context->sb, context->indent);
	append(context->sb, "return s;");
	unindent(context);
	newline(context->sb, context->indent);
	append(context->sb, "} // sum");
	newline(context->sb, context->indent);
	newline(context->sb, context->indent);

/*
	newline(context->sb, context->indent);
	append(context->sb, "double sum(double* a, i32 count) {");
	indent(context);
	newline(context->sb, context->indent);
	append(context->sb, "double s = 0.0;");
	newline(context->sb, context->indent);
	append(context->sb, "for(i32 i = 0; i < count; ++i) {");
	indent(context);
	newline(context->sb, context->indent);
	append(context->sb, "s += a[i];");
	unindent(context);
	newline(context->sb, context->indent);
	append(context->sb, "} //for (i)");
	newline(context->sb, context->indent);
	append(context->sb, "return s;");
	unindent(context);
	newline(context->sb, context->indent);
	append(context->sb, "} // sum");
	newline(context->sb, context->indent);
	newline(context->sb, context->indent);
*/

}

void write_entry_point(cg_context* context) {
	append(context->sb, "// ");
	newline(context->sb, context->indent);
	append(context->sb, "// Program entry point");
	newline(context->sb, context->indent);
	append(context->sb, "// ");
	newline(context->sb, context->indent);
	append(context->sb, "int main(int argc, char** argv) {");
	indent(context);
	newline(context->sb, context->indent);
	append(context->sb, "double a[4] = { 1.0, 2.0, 3.0, 4.0 };");
	newline(context->sb, context->indent);
	append(context->sb, "array_double aa;");
	newline(context->sb, context->indent);
	append(context->sb, "aa.count = sizeof(a) / sizeof(*a);");
	newline(context->sb, context->indent);
	append(context->sb, "aa.elem = a;");
	newline(context->sb, context->indent);
	append(context->sb, "double result = sum(aa);");
	newline(context->sb, context->indent);
	append(context->sb, "printf(\"%f %d\", result, sizeof(i64));");
	newline(context->sb, context->indent);
	append(context->sb, "return 0;");
	unindent(context);
	newline(context->sb, context->indent);
	append(context->sb, "} // main");
	newline(context->sb, context->indent);
	newline(context->sb, context->indent);
}

void vector_codegen(vector_cg_settings settings) {
	fixed_stack_arena<256 * 1204> arena;
	cg_context context;
	
	context.indent = 0;
	context.indent_delta = 4;
	context.arena = &arena;
	context.sb = make_string_buffer(&arena, 128 * 1024);
	context.project_name = settings.project_name;

	newline(context.sb, context.indent);

	//write_include_guard(&context, settings.type_name, true);

	//write_include_file(&context, "arena.h");
	//write_include_file(&context, "util.h");
	write_include_file(&context, "stdio.h");

	newline(context.sb, context.indent);

	//Type definitions...

	append(context.sb, "// Primitive type definitions");
	newline(context.sb, context.indent);
	append(context.sb, "typedef signed char        i8;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef unsigned char      u8;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef short              i16;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef unsigned short     u16;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef int                i32;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef unsigned int       u32;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef long long          i64;");
	newline(context.sb, context.indent);
	append(context.sb, "typedef unsigned long long u64;");
	newline(context.sb, context.indent);

	struct_type st = make_struct(&arena, "array_double", 3);
	set_struct_member_type(&st, 0, make_ptr_type(&arena, make_type("double", type_info_double)));
	set_struct_member_name(&st, 0, "elem");
	set_struct_member_type(&st, 1, make_type("i32", type_info_i32));
	set_struct_member_name(&st, 1, "count");
	set_struct_member_type(&st, 2, make_ptr_type(&arena, make_type("array_double", type_info_struct)));
	set_struct_member_name(&st, 2, "next");

	codegen(&context, st);	

	//Type definitions exported done

	//Function definitions...

	function_header f_add_header = make_function_header(&arena, "add", 4);
	f_add_header.return_type = make_type("void", type_info_void);

	f_add_header.parameters[0].name = "dest";
	f_add_header.parameters[0].type = make_ptr_type(&arena, make_type("double", type_info_double));
	f_add_header.parameters[1].name = "src1";
	f_add_header.parameters[1].type = make_ptr_type(&arena, make_type("double", type_info_double));
	f_add_header.parameters[2].name = "src2";
	f_add_header.parameters[2].type = make_ptr_type(&arena, make_type("double", type_info_double));
	f_add_header.parameters[3].name = "count";
	f_add_header.parameters[3].type = make_type("i32", type_info_i32);

	function f_add = make_function(&arena, f_add_header, 5);

	codegen(&context, f_add);

	write_sum_func(&context);
	write_entry_point(&context);

	//Function definitions done

	//write_include_guard(&context, settings.type_name, false);

	FILE* file = fopen(settings.out_path, "wb");
	if(file) {
		fwrite(context.sb.memory, sizeof(char), context.sb.used, file);
		fclose(file);
	}
}

int main(int argc, char** argv) {
	
	vector_cg_settings settings;

	settings.project_name = "libaxl";
	settings.type_name = "hello_world";
	settings.sub_type_name = "f64";
	settings.out_path = "hello_world.c";

	//Generate code
	vector_codegen(settings);

	{
		fixed_stack_arena<256 * 1204> arena;
		
		value pi = make_double_value(&arena, 3.1415);
		//value pi2 = make_double_value(&arena, 3.1415);
		value two = make_i32_value(&arena, 3);
		value five = make_i32_value(&arena, 7);

		const char* pi_str = to_string(&arena, pi);
		std::cout << "pi: " << pi_str << std::endl;
		const char* two_str = to_string(&arena, two);
		std::cout << "two: " << two_str << std::endl;

		expr e = constant(&arena, pi);

		expr e2 = e * e;
		expr sqrt_of_two = constant(&arena, two) + square_root(constant(&arena, 2.0));
		//expr sqrt_of_two = constant(&arena, 2) + constant(&arena, 3);

		//vector expr syntax:
		//[a + b * c + d] or
		//[a + b * c + d, arena1], [a + c, arena2]

		struct_type st = make_struct(&arena, "array_double", 2);
		set_struct_member_type(&st, 0, make_ptr_type(&arena, make_type("double", type_info_double)));
		set_struct_member_name(&st, 0, "elem");
		set_struct_member_type(&st, 1, make_type("i32", type_info_i32));
		set_struct_member_name(&st, 1, "count");

		cg_context context = make_cg_context(&arena, "libaxl", 1024);

		codegen(&context, st);

		print(context.sb);

		value r = eval(constant(&arena, five) - constant(&arena, two), &arena);//e2.eval_function(&e2, &arena, 0);
		const char* r_str = to_string(&arena, r);
		std::cout << "result: " << r_str << " (" << r.type.type_name << ")" << std::endl;
	}

	system("build_and_run.bat");
	
	int in;
	std::cin >> in;

	return 0;
}
