
#include "../vectors.h"
#include "../expression_tree.h"
#include "../circular_buffer.h"
#include "../stack_arena.h"
#include <iostream>

template <typename T>
void print_vector(libaxl::vector<T> v, bool newline) {
	std::cout << "[";
	auto len = libaxl::length(v);

	if (len > 0)
		std::cout << v.array[0];

	for (int i = 1; i < len; ++i) {
		std::cout << ", " << v.array[v.stride * i];
	}

	std::cout << "]";
	if (newline)
		std::cout << std::endl;
}

int main(int argc, char** argv) {
	using namespace libaxl;
	using vec = vector < double > ;

	libaxl::fixed_stack_arena<1024> arena;
	//libaxl::dynamic_stack_arena arena(1024);

	vec v = ramp_f64(&arena, 5);
	std::cout << length(v) << std::endl;

	print_vector(reverse(v), true);

	std::cout << "Used: " << arena.used() << std::endl;
	vec vres = make_uninitialized_vector<f64>(&arena, 5);
	std::cout << "Used: " << arena.used() << std::endl;
	{
		stack_arena_scope s{ &arena };
		vec v2 = ramp_f64(&arena, 5);
		vec v2_mod = drop(reverse(v2), 1);
		print_vector(v2_mod, true);

		std::cout << length(drop(reverse(v2), 3)) << std::endl;
		std::cout << "Used: " << arena.used() << std::endl;

		vres = copy_to(v2_mod, vres);
	}
	std::cout << "Used: " << arena.used() << std::endl;
	//set(vres, 1, 3.14);
	vres[1] = 3.14;
	print_vector(vres, true);
	
	//print_vector(mean(vres), true);
	//std::cout << "Mean(0): " << to_scalar(mean(vres)) << std::endl;
	//std::cout << "Mean(1): " << to_scalar(mean(vres), 1) << std::endl;
	std::cout << "Used: " << arena.used() << std::endl;

	{
		v64 ones = ones_f64(&arena, length(vres));
		v64 ramp = ramp_f64(&arena, length(vres));
		auto pv = eval(vres + ones * ramp,
			&arena);
		print_vector(pv, true);
	}
	{
		//auto pv = vres * vres - vres;
		//print_vector(pv, true);
	}
	{
		//auto pv = vres * vres + vres;
		//print_vector(pv, true);
	}

	std::cout << "Drop even/odd:" << std::endl;
	std::cout << "original: ";
	print_vector(vres, true);
	std::cout << "drop_even: ";
	print_vector(drop_even(vres), true);
	std::cout << "drop_odd: ";
	print_vector(drop_odd(vres), true);

	vec iota_vec = iota_f64(&arena, 6);
	print_vector(drop_even(iota_vec), true);
	print_vector(drop_odd(iota_vec), true);
	print_vector(drop_even(take(iota_vec, 5)), true);
	print_vector(drop_odd(take(iota_vec, 5)), true);
	print_vector(drop_even(drop(iota_vec, 1)), true);
	print_vector(drop_odd(drop(iota_vec, 1)), true);

	std::cout << std::endl << "... Circular Buffers ..." << std::endl << std::endl;

	circular_buffer<f64> cb = make_circular_buffer(iota_vec, 0);

	for (int i = 1; i <= 6; ++i) {
		std::cout << "read(cb, " << i << "): ";
		print_vector(first(read(cb, i)), false);
		std::cout << ", ";
		print_vector(second(read(cb, i)), true);
	}
	std::cout << std::endl;
	for (int i = 1; i <= 6; ++i) {
		std::cout << "write(cb, " << i << "): ";
		print_vector(first(write(cb, i)), false);
		std::cout << ", ";
		print_vector(second(write(cb, i)), true);
	}
	std::cout << std::endl;

	cb = rotate_left(cb, 2);

	for (int i = 1; i <= 6; ++i) {
		std::cout << "read(cb, " << i << "): ";
		print_vector(first(read(cb, i)), false);
		std::cout << ", ";
		print_vector(second(read(cb, i)), true);
	}
	std::cout << std::endl;
	for (int i = 1; i <= 6; ++i) {
		std::cout << "write(cb, " << i << "): ";
		print_vector(first(write(cb, i)), false);
		std::cout << ", ";
		print_vector(second(write(cb, i)), true);
	}
	std::cout << std::endl;

	cb = rotate_left(cb, 3);

	for (int i = 1; i <= 6; ++i) {
		std::cout << "read(cb, " << i << "): ";
		print_vector(first(read(cb, i)), false);
		std::cout << ", ";
		print_vector(second(read(cb, i)), true);
	}
	std::cout << std::endl;
	for (int i = 1; i <= 6; ++i) {
		std::cout << "write(cb, " << i << "): ";
		print_vector(first(write(cb, i)), false);
		std::cout << ", ";
		print_vector(second(write(cb, i)), true);
	}
	std::cout << std::endl;

	cb = rotate_left(cb, 2);

	for (int i = 1; i <= 6; ++i) {
		std::cout << "read(cb, " << i << "): ";
		print_vector(first(read(cb, i)), false);
		std::cout << ", ";
		print_vector(second(read(cb, i)), true);
	}
	std::cout << std::endl;
	for (int i = 1; i <= 6; ++i) {
		std::cout << "write(cb, " << i << "): ";
		print_vector(first(write(cb, i)), false);
		std::cout << ", ";
		print_vector(second(write(cb, i)), true);
	}
	std::cout << std::endl;

	std::cout << "Used: " << arena.used() << std::endl;
	
	vec filled_vector1 = make_uninitialized_vector<f64>(&arena, 12);
	vec filled_vector2 = make_uninitialized_vector<f64>(&arena, 6);
	fill(filled_vector1, 3.14);
	fill(filled_vector2, -3.14);

	print_vector(filled_vector1, true);
	print_vector(filled_vector2, true);

	int in;
	std::cin >> in;

	return 0;
}