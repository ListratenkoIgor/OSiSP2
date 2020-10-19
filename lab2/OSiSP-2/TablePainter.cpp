#include "TablePainter.h"

inline std::string GetMaxString(Strings content) 
{  
    std::string maxString = content[0][0];
    for (int i = 0; i < content.size(); i++)
    {
        for (int j = 0; j < content[0].size(); j++)
        {
            if (content[i][j].length() > maxString.length())
            {
                maxString = content[i][j];
            }
        }
    }
    return maxString;
}
inline wchar_t* StringToLPCWSTR(std::string str)
{
    int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* text = new wchar_t[length];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, text, length);
    return text;
}
inline void InitRect(int colWidth, int rowHeight, RECT* rect)
{
    rect->left = colWidth + STEP;
    rect->right = rect->left + colWidth - STEP;
    rect->top = rowHeight + STEP;
    rect->bottom = rect->top + rowHeight - STEP;
}

TablePainter::TablePainter() {
    this->_colCount = -1;    
    this->_rowCount = -1;
}
TablePainter::TablePainter(Strings content) {
    this->_colCount = content[0].size();
    this->_rowCount = content.size();
    this->content = content;               
    this->maxString=GetMaxString(content);
}
void TablePainter::SetContent(Strings content) {
    this->_colCount = content[0].size();
    this->_rowCount = content.size();
    this->content = content;
    this->maxString = GetMaxString(content);
}
BOOL TablePainter::DrawLine(HDC hdc, int x1, int y1, int x2, int y2) {
    POINT pt;
    MoveToEx(hdc, x1, y1, &pt);
    return LineTo(hdc, x2, y2);
}                
void TablePainter::DrawTable(HDC hdc, int width, int height)
{
    int colWidth = width / _colCount;
    int rowHeight = height / _rowCount;
    for (int i = 1; i < _colCount; i++)
    {
        int x = colWidth * i;
        DrawLine(hdc, x, 0, x, height/*rowHeight*_rowCount*/);
    }
    for (int i = 1; i < _rowCount; i++)
    {
        int y = rowHeight * i;
        DrawLine(hdc, 0, y, width/*colWidth*_colCount*/, y);
    }
    DrawCells(hdc, width, height);
}
void TablePainter::DrawCells(HDC hdc, int width, int height)
{
    RECT rect;
    HFONT hfont;
    RECT tempRect;
    HFONT oldFont;
    LPCWSTR text = StringToLPCWSTR(this->maxString);    
    BOOL temp = FALSE;
    int colWidth = width / _colCount;
    int rowHeight = height / _rowCount;
    HDC hmemDC = CreateCompatibleDC(hdc);
    if (colWidth > MIN_WIDTH) {
        hfont = CreateFont(rowHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, FONT);
        oldFont = (HFONT)SelectObject(hmemDC, hfont);
        InitRect(colWidth, rowHeight, &rect);
        InitRect(colWidth, rowHeight, &tempRect);                                                   
        DrawText(hmemDC, text, -1, &tempRect, DT_CALCRECT | DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
        hfont = (HFONT)SelectObject(hmemDC, oldFont);
        temp = DeleteObject(hfont);
        int fontHeight = rowHeight;
        while (tempRect.right > rect.right) {
            InitRect(colWidth, rowHeight, &tempRect);
            fontHeight -= STEP;
            hfont = CreateFont(fontHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_SWISS, FONT);
            oldFont = (HFONT)SelectObject(hmemDC, hfont);
            DrawText(hmemDC, text, -1, &tempRect, DT_CALCRECT | DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
            hfont = (HFONT)SelectObject(hmemDC, oldFont);
            temp = DeleteObject(hfont);
        }
        temp = DeleteDC(hmemDC);
        hfont = CreateFont(fontHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, FONT);
        oldFont = (HFONT)SelectObject(hdc, hfont);
        for (int i = 0; i < _rowCount; i++)
        {
            for (int j = 0; j < _colCount; j++)
            {
                DrawCell(hdc, colWidth, rowHeight, i, j);
            }
        }
        SelectObject(hdc, oldFont);
        DeleteObject(hfont);
    }
}
void TablePainter::DrawCell(HDC hdc, int colWidth, int rowHeight, int i, int j)
{
    RECT rect;
    rect.left = colWidth * j + 1;
    rect.right = rect.left + colWidth - 1;
    rect.top = rowHeight * i + 1;
    rect.bottom = rect.top + rowHeight - 1;
    DrawText(hdc, StringToLPCWSTR(content[i][j]), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
}