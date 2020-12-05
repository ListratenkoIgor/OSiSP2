#pragma once

#include "CommonHeaders.h"
#include "ConcurrentQueue.h"
#include "Task.h"

typedef void (CALLBACK* Work_Callback_Function)(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work);

class Threadpool {
public:
    ConcurrentQueue<Task>* queue;
    std::vector<std::string>* buffer;

    Threadpool(ConcurrentQueue<Task>* , std::vector<std::string>* );
    ~Threadpool();

    void SetTreadPoolWork(Work_Callback_Function);
    void SetThreadsCount(int);
    void Process();
    void Wait();

private:
    int ThreadsCount;
    PTP_POOL pool;
    PTP_CLEANUP_GROUP cleanupGroup;
    TP_CALLBACK_ENVIRON callbackEnvironment;
    Work_Callback_Function  WorkCallback;

};
