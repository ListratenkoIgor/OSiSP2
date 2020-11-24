#pragma once
#include "CommonHeaders.h"
typedef struct {
	long startOffset;
	long endOffset;
}Parametrs;

typedef void (*Task_Function)(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end);

typedef struct {
	Parametrs parametrs;
	Task_Function task;
}Task, * PTask;
