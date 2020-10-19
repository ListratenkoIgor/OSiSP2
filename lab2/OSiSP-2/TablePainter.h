#pragma once
#include "types.h"
#include "MemoryWatcher.h"

class TablePainter
{
private:
    int _colCount;
    int _rowCount;
    Strings content;   
    std::string maxString;
public:
    TablePainter();
    TablePainter(Strings);
    void SetContent(Strings);
    void DrawTable(HDC, int, int);
    BOOL DrawLine(HDC, int, int, int, int);
    void DrawCells(HDC, int, int);
    void DrawCell(HDC, int, int, int, int);
};
