#pragma once
#include "types.h"
#include <psapi.h>
#include <thread>
#include <iostream>
class MemoryWatcher
{
private:
	std::string _path;
public:	
	MemoryWatcher(std::string);
	void WriteMemory(bool, std::string);
};