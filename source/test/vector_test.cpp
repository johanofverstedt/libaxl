
#include "../vectors.h"
#include "../circular_buffer.h"
#include "../stack_arena.h"
#include <iostream>

void print_vector(libaxl::vector v, bool newline) {
	std::cout << "[";
	auto len = libaxl::length(v);

	if (v.width == 1) {
		if (len > 0)
			std::cout << v.array[0];

		for (int i = 1; i < len; ++i) {
			std::cout << ", " << v.array[v.stride * i];
		}
	}
	else {
		if (len > 0) {
			std::cout << "(";
			std::cout << v.array[0];
			for (int j = 1; j < v.width; ++j)
				std::cout << ", " << v.array[j];
			std::cout << ")";
		}
		for (int i = 1; i < len; ++i) {
			std::cout << ", (";
			std::cout << v.array[i * v.stride];

			for (int j = 1; j < v.width; ++j) {
				std::cout << ", " << v.array[i * v.stride + j];
			}
			std::cout << ")";
		}
	}

	std::cout << "]";
	if (newline)
		std::cout << std::endl;
}

int main(int argc, char** argv) {
	using namespace libaxl;

	libaxl::fixed_stack_arena<1024> arena;
	//libaxl::dynamic_stack_arena arena(1024);

	vector v = ramp(&arena, 5, 1);
	std::cout << length(v) << std::endl;

	print_vector(reverse(v), true);

	std::cout << "Used: " << arena.used() << std::endl;
	vector vres = make_uninitialized_vector(&arena, 5, 2);
	std::cout << "Used: " << arena.used() << std::endl;
	{
		stack_arena_scope s{ &arena };
		vector v2 = ramp(&arena, 5, 2);
		vector v2_mod = drop(reverse(v2), 1);
		print_vector(v2_mod, true);

		std::cout << length(drop(reverse(v2), 3)) << std::endl;
		std::cout << "Used: " << arena.used() << std::endl;

		vres = copy_to(v2_mod, vres);
	}
	std::cout << "Used: " << arena.used() << std::endl;
	set(vres, 1, 1, 3.14);
	print_vector(vres, true);
	
	print_vector(mean(vres), true);
	std::cout << "Mean(0): " << to_scalar(mean(vres)) << std::endl;
	std::cout << "Mean(1): " << to_scalar(mean(vres), 1) << std::endl;
	std::cout << "Used: " << arena.used() << std::endl;

	print_vector(vres + ones(&arena, length(vres), vres.width), true);
	print_vector(vres * vres - vres, true);
	print_vector(vres * vres + vres, true);

	std::cout << "Drop even/odd:" << std::endl;
	std::cout << "original: ";
	print_vector(vres, true);
	std::cout << "drop_even: ";
	print_vector(drop_even(vres), true);
	std::cout << "drop_odd: ";
	print_vector(drop_odd(vres), true);

	vector iota_vec = iota(&arena, 6);
	print_vector(drop_even(iota_vec), true);
	print_vector(drop_odd(iota_vec), true);
	print_vector(drop_even(take(iota_vec, 5)), true);
	print_vector(drop_odd(take(iota_vec, 5)), true);
	print_vector(drop_even(drop(iota_vec, 1)), true);
	print_vector(drop_odd(drop(iota_vec, 1)), true);

	std::cout << std::endl << "... Circular Buffers ..." << std::endl << std::endl;

	circular_buffer cb = make_circular_buffer(iota_vec, 0);

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
	
	vector filled_vector1 = make_uninitialized_vector(&arena, 12, 1);
	vector filled_vector2 = make_uninitialized_vector(&arena, 6, 2);
	fill(filled_vector1, 3.14);
	fill(filled_vector2, -3.14);

	print_vector(filled_vector1, true);
	print_vector(filled_vector2, true);

	int in;
	std::cin >> in;

	return 0;
}