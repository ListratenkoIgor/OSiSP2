#pragma once
#include "types.h"

class TablePainter
{
private:
    int _colCount;
    int _rowCount;
    Strings content;   
    std::string maxString;
    HDC hdc;
public:
    TablePainter(HDC);
    TablePainter(HDC,Strings);
    ~TablePainter();
    void SetContent(Strings);
    void DrawTable(HDC, int, int);
    BOOL DrawLine(HDC, int, int, int, int);
    void DrawCells(HDC, int, int);
    void DrawCell(HDC, int, int, int, int);
    std::string GetMaxString(Strings);
};
