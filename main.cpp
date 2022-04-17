#include "thread_pool.hpp"
#include <thread>
#include <iostream>
using namespace rdx;

unsigned int fibonacci(unsigned int num) {
	if (num == 0) { // Die Fibonacci-Zahl von null ist null
		return 0;
	} // else
	if (num == 1) { // Die Fibonacci-Zahl von eins ist eins
		return 1;
	} // else

	// Ansonsten wird die Summe der zwei vorherigen Fibonacci-Zahlen zurückgegeben.
	return fibonacci(num - 1) + fibonacci(num - 2);
}

int main() {
	srand(time(NULL));
	thread_pool pool(4);
	std::mutex m;
	pool.enqueue([&m]() {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
		auto val = fibonacci(32);
		std::unique_lock<std::mutex> lock(m);

		std::cout << "d : " << val << std::endl;
	});
	pool.enqueue([&m]() {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
		auto val = fibonacci(32);
		std::unique_lock<std::mutex> lock(m);

		std::cout << "d : " << val << std::endl;
	});
	pool.enqueue([&m]() {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
		auto val = fibonacci(32);
		std::unique_lock<std::mutex> lock(m);

		std::cout << "d : " << val << std::endl;
	});
	pool.enqueue([&m]() {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
		auto val = fibonacci(48);
		std::unique_lock<std::mutex> lock(m);
		std::cout << "d : " << val << std::endl;
	});
	pool.join();
}