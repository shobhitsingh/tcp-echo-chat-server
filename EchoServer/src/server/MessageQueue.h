/*
 * MessageQueue.h
 *
 *  Created on: Mar 14, 2016
 *      Author: shobhit
 */

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace es {

template <typename T>
class MessageQueue
{
private:
	MessageQueue(const MessageQueue&);
	MessageQueue& operator=(const MessageQueue&);

public:
	MessageQueue(): queue(), mutex(), cond(){}
	T Pop()
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (queue.empty())
		{
			cond.wait(lock);
		}
		T val = queue.front();
		queue.pop();
		return val;
	}

	void Push(const T& item)
	{
		std::unique_lock<std::mutex> lock(mutex);
		queue.push(item);
		lock.unlock();
		cond.notify_one();
	}

private:
	std::queue<T> queue;
	mutable std::mutex mutex;
	std::condition_variable cond;
};


}


#endif /* MESSAGEQUEUE_H_ */
