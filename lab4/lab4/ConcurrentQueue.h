#pragma once
#include "CommonHeaders.h"
#include "Task.h"

class ConcurrentQueue
{
	template<template<class>typename T> std::queue<T> concurrentQueue;
	CRITICAL_SECTION popCriticalSection;
	CRITICAL_SECTION pushCriticalSection;
public: 
	ConcurrentQueue();
	~ConcurrentQueue();
	template <typename T> void Enqueue(T);
	template <typename T> T Dequeue();
};

