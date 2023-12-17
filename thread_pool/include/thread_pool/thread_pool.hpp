#ifndef RDX_THREAD_POOL_HPP
#define RDX_THREAD_POOL_HPP
#include <cstddef>
#include <future>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
namespace rdx {

	/*
		/brief A thread pool that spawns a specified number of workers.

		Worker threads wait for the queue to be filled with tasks.
		Enqueueing tasks return futures so that they can return asynchronous values.
		Destructor will join all workers.
	*/
	class thread_pool {
	private:
		std::queue<std::function<void()>> task_queue;
		std::mutex queue_mutex;
		std::condition_variable queue_notification;
		std::vector<std::thread> workers;
		bool should_stop;
		void worker_function();
	public:
		// constructs a thread pool with the given number of workers
		thread_pool(std::size_t num_workers = std::thread::hardware_concurrency());
		~thread_pool();
		//enqueues a task to be performed and returns a future for that task
		template<class F, class... Args>
		std::future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args);

		thread_pool(thread_pool&&) = delete;
		thread_pool(const thread_pool&) = delete;
		thread_pool& operator=(thread_pool&&) = delete;
		thread_pool& operator=(const thread_pool&) = delete;
	};

	template<class F, class ...Args>
	inline std::future<typename std::result_of<F(Args ...)>::type> thread_pool::enqueue(F&& f, Args && ...args) {
		using return_type = typename std::result_of<F(Args...)>::type;
		auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			task_queue.emplace([task]() { (*task)(); });
		}
		queue_notification.notify_one();
		return res;
	}

}
#endif // !RDX_THREAD_POOL_H


