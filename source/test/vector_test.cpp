
#include "../vectors.h"
#include "../stack_allocator.h"
#include <iostream>

void print_vector(libaxl::vector v, bool newline) {
	std::cout << "[";
	auto len = libaxl::length(v);

	if (v.width == 1) {
		if (len > 0)
			std::cout << v.array[v.start];

		for (int i = 1; i < len; ++i) {
			std::cout << ", " << v.array[v.start + v.stride * i];
		}
	}
	else {
		if (len > 0) {
			std::cout << "(";
			std::cout << v.array[v.start];
			for (int j = 1; j < v.width; ++j)
				std::cout << ", " << v.array[v.start + j];
			std::cout << ")";
		}
		for (int i = 1; i < len; ++i) {
			std::cout << ", (";
			std::cout << v.array[v.start + i * v.stride];

			for (int j = 1; j < v.width; ++j) {
				std::cout << ", " << v.array[v.start + i * v.stride + j];
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

	libaxl::stack_vector_allocator<1024> allocator;
	//libaxl::dynamic_stack_vector_allocator allocator(1024);

	vector v = ramp(&allocator, 5, 1);
	std::cout << length(v) << std::endl;

	print_vector(reverse(v), true);

	std::cout << "Used: " << allocator.used() << std::endl;
	vector vres = make_uninitialized_vector(&allocator, 5, 2);
	std::cout << "Used: " << allocator.used() << std::endl;
	{
		alloc_scope s{ &allocator };
		vector v2 = ramp(&allocator, 5, 2);
		vector v2_mod = drop(reverse(v2), 1);
		print_vector(v2_mod, true);

		std::cout << length(drop(reverse(v2), 3)) << std::endl;
		std::cout << "Used: " << allocator.used() << std::endl;

		vres = copy_to(v2_mod, vres);
	}
	std::cout << "Used: " << allocator.used() << std::endl;
	set(vres, 1, 1, 3.14);
	print_vector(vres, true);
	
	print_vector(mean(vres), true);
	std::cout << "Mean(0): " << to_scalar(mean(vres)) << std::endl;
	std::cout << "Mean(1): " << to_scalar(mean(vres), 1) << std::endl;
	std::cout << "Used: " << allocator.used() << std::endl;

	int in;
	std::cin >> in;

	return 0;
}