#pragma once
#include <windows.h>
#include <queue>
class ConcurrentQueue
{
	std::queue<int> concurrentQueue;
public: 
	ConcurrentQueue();
	void Enqueue();
	int Dequeue();
};

