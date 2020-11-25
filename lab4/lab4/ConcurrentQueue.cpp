#include "ConcurrentQueue.h"
/*
template<typename T>
ConcurrentQueue<T>::ConcurrentQueue() {
    this->concurrentQueue = new std::queue<T>();
    InitializeCriticalSection(&popCriticalSection);
    InitializeCriticalSection(&pushCriticalSection);
}

ConcurrentQueue::~ConcurrentQueue() {
    DeleteCriticalSection(&popCriticalSection);
    DeleteCriticalSection(&pushCriticalSection);
}

template <typename T>
void ConcurrentQueue::Enqueue(T element) {
    EnterCriticalSection(&pushCriticalSection);

    this->concurrentQueue.push(element);

    LeaveCriticalSection(&pushCriticalSection);
}
template <typename T>
T ConcurrentQueue::Dequeue() {

    EnterCriticalSection(&popCriticalSection);
    PTask result = NULL;
    if (!this->concurrentQueue.empty()) {
        result = this->concurrentQueue.front();
        this->concurrentQueue.pop();
    }
    LeaveCriticalSection(&popCriticalSection);

    return result;
}

*/