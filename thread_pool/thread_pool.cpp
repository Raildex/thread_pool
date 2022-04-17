#include "thread_pool.hpp"

namespace rdx {
	void thread_pool::worker_function() {
		while (true) {
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				condition.wait(lock, [this]() {
					return should_join || should_stop || !task_queue.empty();
				});
				if (should_stop || (task_queue.empty() && should_join)) {
					break;
				}
				task = task_queue.front();
				task_queue.pop();
			}
			task();
		}
	}

	thread_pool::thread_pool(std::size_t num_workers) : should_stop(false), should_join(false) {
		for (auto i = 0; i < num_workers; i++) {
			workers.emplace_back(&thread_pool::worker_function,this);
		}
	}

	thread_pool::~thread_pool() {
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			should_stop = true;
			should_join = true;
		}
		join();
	}
	void thread_pool::join() {
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			should_join = true;
		}
		condition.notify_all();
		for (auto& worker : workers) {
			if (worker.joinable()) {
				worker.join();
			}
		}
	}
}

