#include "ConcurrentQueue.h"


ConcurrentQueue::ConcurrentQueue() {
    InitializeCriticalSection(&popCriticalSection);
    InitializeCriticalSection(&pushCriticalSection);
}

ConcurrentQueue::~ConcurrentQueue() {
    DeleteCriticalSection(&popCriticalSection);
    DeleteCriticalSection(&pushCriticalSection);
}

template <typename T>
void ConcurrentQueue<>::Enqueue(T element) {
    EnterCriticalSection(&pushCriticalSection);

    this->concurrentQueue.push(element);

    LeaveCriticalSection(&pushCriticalSection);
}
template <typename T>
T ConcurrentQueue<T>::Dequeue() {

    EnterCriticalSection(&popCriticalSection);
    PTask result = NULL;
    if (!this->concurrentQueue.empty()) {
        result = this->concurrentQueue.front();
        this->concurrentQueue.pop();
    }
    LeaveCriticalSection(&popCriticalSection);

    return result;
}

