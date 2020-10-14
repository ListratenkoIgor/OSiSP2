#pragma once
#include "types.h"

using namespace std;
class FileReader
{

public:
    FileReader();
    Strings GetContentFromFile(string); 
    Strings GetContentFromFile(TCHAR);
};
