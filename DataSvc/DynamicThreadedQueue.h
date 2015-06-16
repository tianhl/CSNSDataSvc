#ifndef DYNAMIC_THREADED_QUEUE_h 
#define DYNAMIC_THREADED_QUEUE_h 
#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <queue>

template<typename T>
class DynamicThreadedQueue
:boost::noncopyable
{
	public:
		DynamicThreadedQueue() : queue_(), cond_(), mutex_() {}
		~DynamicThreadedQueue(){}

		void put(const T& obj) {
			boost::unique_lock<boost::mutex> lock(mutex_); 
			queue_.push(obj);
			cond_.notify_all();
		}

		T get() {
			boost::unique_lock<boost::mutex> lock(mutex_); 
			while(queue_.size()== 0) cond_.wait(lock);
			T front(queue_.front());
			queue_.pop();
			return front;
		}

		//T get(bool block = true) {
		//	boost::unique_lock<boost::mutex> lock(mutex_); 
		//	if(block) {
		//		while(queue_.size()== 0) cond_.wait(lock);
		//	}
		//	else if(0 == queie_.size()) return NULL; 
		//	T front(queue_.front());
		//	queue_.pop();
		//	return front;
		//}

		unsigned size() { return queue_.size(); }

		void notify_all() { cond_.notify_all(); }
	private:
		std::queue<T> queue_;
		boost::condition_variable_any cond_;
		boost::mutex mutex_;
};


#endif
