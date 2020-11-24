// lab4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include "ThreadPool.h"
#include "ConcurrentQueue.h"
#include "Task.h"
#define FILE_PATH_TO_READ  "D:\\5sem\\OSISP\\OSISP\\Lab4\\test.txt"
#define FILE_PATH_TO_WRITE  "D:\\5sem\\OSISP\\OSISP\\Lab4\\out.txt"
#define MAX_STRING_SIZE 1024

void ThreadWork(PTP_CALLBACK_INSTANCE instance, PVOID parameter, PTP_WORK work) {
    auto* pool = reinterpret_cast<Threadpool*>(parameter);
    Task task = pool->queue->Dequeue();
    int startIndex = task.parametrs.startOffset - 1;
    int finishIndex = task.parametrs.endOffset;
    (task.task)(pool->buffer->begin() + startIndex, pool->buffer->begin() + finishIndex);
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
            queue->Enqueue({ {stringCountForThread * i + 1, stringCountForThread * (i + 1)},Sort });
        }
        else {
            queue->Enqueue({ stringCountForThread * i + 1, stringCountForThread * (i + 1) + modCount,Sort });
        }
    }


    //___________________________________________________________________________________________________________________


    Threadpool* pool = new Threadpool(queue, &buffer);
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

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
