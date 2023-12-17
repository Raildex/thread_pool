#include "thread_pool/thread_pool.hpp"

namespace rdx {
	void thread_pool::worker_function() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            while(task_queue.empty()) {
                if(should_stop) {
                    return;
                }
                queue_notification.wait(lock);
            }
            task = std::move(task_queue.front());
            task_queue.pop();
        }
        task();
    }
}

	thread_pool::thread_pool(std::size_t num_workers) : should_stop(false) {
		workers.reserve(num_workers);
		for (auto i = 0; i < num_workers; i++) {
			workers.emplace_back(&thread_pool::worker_function,this);
		}
	}

	thread_pool::~thread_pool() {
		{
			std::lock_guard<std::mutex> lock(queue_mutex);
			should_stop = true;
		}
		queue_notification.notify_all();
		for (auto& worker : workers) {
			if(worker.joinable()) {
				worker.join();
			}
		}
	}
}

