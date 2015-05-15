#ifndef THREAD_QUEUE_h 
#define THREAD_QUEUE_h 
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <queue>

template<typename T>
class ThreadQueue
:boost::noncopyable
{
	public:
		ThreadQueue() : queue_(), cond_(), mutex_() {}
		~ThreadQueue(){}

		void put(const T& obj) {
			boost::unique_lock<boost::mutex> lock(mutex_); 
			queue_.push(obj);
			cond_.notify_all();
		}

		T get() {
			boost::unique_lock<boost::mutex> lock(mutex_); 
			if (queue_.size() == 0) cond_.wait(lock);
			T front(queue_.front());
			queue_.pop();
			return front;
		}

		unsigned size() { return queue_.size(); }

		void notify_all() { cond_.notify_all(); }
	private:
		std::queue<T> queue_;
		boost::condition_variable_any cond_;
		boost::mutex mutex_;
};


#endif
