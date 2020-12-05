#include <iostream>
#include <fstream>
#include "CommonHeaders.h"
#include "ThreadPool.h"
#include "ConcurrentQueue.h"
#include "Task.h"
#define FILE_PATH_TO_READ  "E:\\test.txt"
#define FILE_PATH_TO_WRITE  "E:\\out.txt"
#define MAX_STRING_SIZE 1024

void ThreadWork(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work) {
    try {
        auto* pool = reinterpret_cast<Threadpool*>(parameter);
        Task task = pool->queue->Dequeue();
        if (NULL == task.task) {
            return;
        }
        int startIndex = task.parametrs.startOffset - 1;
        int finishIndex = task.parametrs.endOffset;
        (task.task)(pool->buffer->begin() + startIndex, pool->buffer->begin() + finishIndex);
    }
    catch(...){                          
    }
}

void Sort(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end) {
    sort(begin, end);
}

int main()
{
    std::vector<std::string> buffer;

    std::cout << "Enter count of the threads" << std::endl;
    int countOfThreadsFromConsole;
    std::cin >> countOfThreadsFromConsole;

    int maxThreads = std::thread::hardware_concurrency();
    if (countOfThreadsFromConsole > maxThreads) {
        std::cout << "Max number of threads is " << maxThreads << std::endl;
        return 1;
    }

    std::ifstream fin(FILE_PATH_TO_READ);
    long stringCount = 0;
    char str[MAX_STRING_SIZE];

    int j = 0;
    while (!fin.eof()) {
        fin.getline(str, MAX_STRING_SIZE);
        stringCount++;
        buffer.resize(stringCount);
        buffer[j] = str;
        if (buffer[j][buffer[j].size() - 1] != '\n') {
            buffer[j] += '\n';
        }
        j++;
    }
    fin.close();

    //___________________________________________________________________________________________________________________
    ConcurrentQueue<Task>* queue = new ConcurrentQueue<Task>();

    int countOfThreads;
    long stringCountForThread;
    int modCount;
    if (stringCount > countOfThreadsFromConsole) {
        countOfThreads = countOfThreadsFromConsole;
        stringCountForThread = stringCount / countOfThreadsFromConsole;
        modCount = stringCount % countOfThreadsFromConsole;
    }
    else {
        stringCountForThread = 1;
        modCount = 0;
        countOfThreads = stringCount;
    }
    for (int i = 0; i < countOfThreads; i++) {
        if (i != countOfThreads - 1) {
            queue->Enqueue({ {stringCountForThread * i + 1, stringCountForThread * (i + 1)},(Task_Function)Sort });
        }
        else {
            queue->Enqueue({ stringCountForThread * i + 1, stringCountForThread * (i + 1) + modCount,(Task_Function)Sort });
        }
    }


    //___________________________________________________________________________________________________________________

    Threadpool* pool = new Threadpool(queue, &buffer);
    pool->SetTreadPoolWork(ThreadWork);
    pool->SetThreadsCount(countOfThreads);
    pool->Process();
    pool->Wait();

    //___________________________________________________________________________________________________________________
    int countSort = stringCount / stringCountForThread - 1;
    for (int i = 0; i < countSort; i++) {
        if (i == (countSort - 1)) {
            std::inplace_merge(buffer.begin(), buffer.begin() + stringCountForThread * (i + 1),
                buffer.end());
        }
        else {
            std::inplace_merge(buffer.begin(), buffer.begin() + stringCountForThread * (i + 1),
                buffer.begin() + stringCountForThread * (i + 2));
        }
    }

    std::ofstream fout(FILE_PATH_TO_WRITE);
    for (int i = 0; i < buffer.size(); i++) {
        fout << buffer[i];
    }
    fout.close();

    return 0;

}
