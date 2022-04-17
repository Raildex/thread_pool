#ifndef RDX_THREAD_POOL_HPP
#define RDX_THREAD_POOL_HPP
#include <cstddef>
#include <future>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
namespace rdx {

	/*
		/brief A thread pool that spawns a specified number of workers.

		Worker threads wait for the queue to be filled with tasks.
		Enqueueing tasks return futures so that they can return asynchronous values.
		Call join() to wait for all pending tasks to complete
		if the destructor is called, it will abort all pending tasks.
	*/
	class thread_pool {
	private:
		std::queue<std::function<void()>> task_queue;
		std::mutex queue_mutex;
		std::condition_variable condition;
		std::vector<std::thread> workers;
		bool should_stop;
		bool should_join;
		void worker_function();
	public:
		// constructs a thread pool with the given number of workers
		thread_pool(std::size_t num_workers);
		~thread_pool();
		//enqueues a task to be performed and returns a future for that task
		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args) ->std::future<typename std::result_of<F(Args...)>::type>;
		// Joins the thread pool and it's workers.
		void join();
	};


	template<class F, class ...Args>
	inline auto thread_pool::enqueue(F&& f, Args && ...args) -> std::future<typename std::result_of<F(Args ...)>::type> {
		using return_type = typename std::result_of<F(Args...)>::type;
		auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			task_queue.emplace([task]() { (*task)(); });
		}
		condition.notify_one();
		return res;
	}
	
}
#endif // !RDX_THREAD_POOL_H


