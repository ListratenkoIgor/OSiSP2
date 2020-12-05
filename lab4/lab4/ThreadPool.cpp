#include "ThreadPool.h"

#include <Windows.h>
#include <iostream>
#include <queue>

Threadpool::Threadpool(ConcurrentQueue<Task>* queue, std::vector<std::string>* buffer) {
    InitializeThreadpoolEnvironment(&this->callbackEnvironment);

    this->pool = CreateThreadpool(nullptr);
    SetThreadpoolThreadMinimum(this->pool, 1);

    this->cleanupGroup = CreateThreadpoolCleanupGroup();
    SetThreadpoolCallbackCleanupGroup(&this->callbackEnvironment, this->cleanupGroup, nullptr);
    this->ThreadsCount = 0;
    this->queue = queue;
    this->buffer = buffer;
    this->WorkCallback = NULL;
}

Threadpool::~Threadpool() {

    CloseThreadpoolCleanupGroup(this->cleanupGroup);
    CloseThreadpool(this->pool);
}

void Threadpool::SetThreadsCount(int threadCount) {
    this->ThreadsCount = threadCount;
    SetThreadpoolThreadMaximum(this->pool, threadCount);
}

void Threadpool::SetTreadPoolWork(Work_Callback_Function WorkCallback) {
    this->WorkCallback = WorkCallback;
}

void Threadpool::Process() {
    if (WorkCallback) {
        PTP_WORK work = CreateThreadpoolWork(this->WorkCallback, this, &this->callbackEnvironment);
        for (int i = 0; i < this->ThreadsCount; i++) {
            SubmitThreadpoolWork(work);
        }
    }
}

void Threadpool::Wait() {
    CloseThreadpoolCleanupGroupMembers(this->cleanupGroup, false, nullptr);
}
