#include "thread_pool.hpp"
#include <thread>
#include <iostream>
using namespace rdx;

unsigned int fibonacci(unsigned int num) {
	if (num == 0) {
		return 0;
	}
	if (num == 1) {
		return 1;
	}
	return fibonacci(num - 1) + fibonacci(num - 2);
}


void parallel() {
	std::chrono::nanoseconds concurrent = std::chrono::nanoseconds(0);
	for (int i = 0; i < 300; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		{
			thread_pool pool{};
			start = std::chrono::high_resolution_clock::now();
			pool.enqueue([]() {
				auto val = fibonacci(32);
				});
			pool.enqueue([]() {
				auto val = fibonacci(32);
				});
			pool.enqueue([]() {
				auto val = fibonacci(32);
				});
			pool.enqueue([]() {
				auto val = fibonacci(32);
				});
		}
		auto end = std::chrono::high_resolution_clock::now();
		concurrent += end - start;
	}
	std::wcout << L"Parallel : Ø\t" << concurrent.count() / 300 << L"ns" << std::endl;
}


void serial() {
	std::chrono::nanoseconds serial = std::chrono::nanoseconds(0);

	for (int i = 0; i < 300; i++) {
		auto start = std::chrono::high_resolution_clock::now();

		auto val = fibonacci(32);
		val = fibonacci(32);
		val = fibonacci(32);
		val = fibonacci(32);
		auto end = std::chrono::high_resolution_clock::now();
		serial += end - start;

	}
	std::wcout << L"Serial : Ø\t" << serial.count() / 300 << L"ns" << std::endl;

}

int main() {
	serial();
	parallel();
}