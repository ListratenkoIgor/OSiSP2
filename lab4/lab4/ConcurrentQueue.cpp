#include "ConcurrentQueue.h"

ConcurrentQueue::ConcurrentQueue() {
    InitializeCriticalSection(&popCriticalSection);
    InitializeCriticalSection(&pushCriticalSection);
}

ConcurrentQueue::~ConcurrentQueue() {
    DeleteCriticalSection(&popCriticalSection);
    DeleteCriticalSection(&pushCriticalSection);
}

void ConcurrentQueue::Enqueue(PTask task) {
    EnterCriticalSection(&pushCriticalSection);

    this->concurrentQueue.push(task);

    LeaveCriticalSection(&pushCriticalSection);
}
PTask ConcurrentQueue::Dequeue() {

    EnterCriticalSection(&popCriticalSection);
    PTask result = NULL;
    if (!this->concurrentQueue.empty()) {
        result = this->concurrentQueue.front();
        this->concurrentQueue.pop();
    }
    LeaveCriticalSection(&popCriticalSection);

    return result;
}

