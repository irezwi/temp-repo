#include "../ExtendedPrecission.hpp"
#include <chrono>
#include <iostream>

void test_add() {
	ExtendedPrecission ep(-1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0));
	ExtendedPrecission pe(-1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0));
	ExtendedPrecission result(0.0);
	result = ep + pe;
}

void test_subtract() {
	ExtendedPrecission ep(-1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0));
	ExtendedPrecission pe(-1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0));
	ExtendedPrecission result(0.0);
	result = ep - pe;
}

void test_multiply() {
	ExtendedPrecission ep((double)rand() / RAND_MAX);
	ExtendedPrecission pe((double)rand() / RAND_MAX);
	ExtendedPrecission result(0.0);
	result = ep * pe;
}

int main() {
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; ++i) {
		test_add();
	}
	auto end = std::chrono::steady_clock::now();
	auto delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "test_add elapsed time: " << delta_time / 1000000 << std::endl;

	start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; ++i) {
		test_subtract();
	}
	end = std::chrono::steady_clock::now();
	delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "test_subtract elapsed time: " << delta_time / 1000000 << std::endl;

	start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; ++i) {
		test_multiply();
	}
	end = std::chrono::steady_clock::now();
	delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "add_multiply elapsed time: " << delta_time / 1000000 << std::endl;

	start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; ++i) {
		double a = -1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0);
		double b = -1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0);
		double c = a + b;
	}
	end = std::chrono::steady_clock::now();
	delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "add_test elapsed time: " << delta_time / 1000000 << std::endl;

	start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; ++i) {
		double a = -1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0);
		double b = -1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0);
		double c = a - b;
	}
	end = std::chrono::steady_clock::now();
	delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "subtract_test elapsed time: " << delta_time / 1000000 << std::endl;

	start = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000000; ++i) {
		double a = -1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0);
		double b = -1000.0 + (double)rand() / RAND_MAX * (1000.0 - 2000.0);
		double c = a * b;
	}
	end = std::chrono::steady_clock::now();
	delta_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "multiply_test elapsed time: " << delta_time / 1000000 << std::endl;
}
