#pragma once
#include "CommonHeaders.h"
#include "Task.h"
template<typename T>
class ConcurrentQueue
{
	//template <class T, class _Container = std::deque<T>>

	std::queue<T> concurrentQueue;
	CRITICAL_SECTION popCriticalSection;
	CRITICAL_SECTION pushCriticalSection;
public: 
	ConcurrentQueue() {
		this->concurrentQueue = *new std::queue<T>();
		InitializeCriticalSection(&popCriticalSection);
		InitializeCriticalSection(&pushCriticalSection);
	};
	~ConcurrentQueue() {
		DeleteCriticalSection(&popCriticalSection);
		DeleteCriticalSection(&pushCriticalSection);
	};
	void Enqueue(T element) {
		EnterCriticalSection(&pushCriticalSection);

		this->concurrentQueue.push(element);

		LeaveCriticalSection(&pushCriticalSection);
	};
	T Dequeue() {
		EnterCriticalSection(&popCriticalSection);
		T result = *new T();
		ZeroMemory(&result, sizeof(result));
		if (!this->concurrentQueue.empty()) {
			result = (T)this->concurrentQueue.front();
			this->concurrentQueue.pop();
		}
		LeaveCriticalSection(&popCriticalSection);

		return result;
	};
	bool IsEmpty() {
		return this->concurrentQueue.empty();
	}
};


