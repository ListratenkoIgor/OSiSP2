#include "TablePainter.h"


wchar_t* StringToLPCWSTR(std::string str)
{
    int length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* text = new wchar_t[length];
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, text, length);
    return text;
}



std::string TablePainter::GetMaxString(Strings content)
{  
    std::string maxString = content[0][0];
    SIZE maxSize = {0,0};    
    for (int i = 0; i < content.size(); i++)
    {
        for (int j = 0; j < content[0].size(); j++)
        {
            //if (content[i][j].length() >= maxString.length())
            {
                SIZE size;
                wchar_t* str = StringToLPCWSTR(content[i][j]);
                int length = content[i][j].length();
                if (GetTextExtentPoint32(this->hdc, str, length , &size)) {
                    if (size.cx > maxSize.cx) {
                        maxSize.cx = size.cx;
                        maxString = content[i][j];
                    }
                }
                else {
                    maxString = content[i][j];
                }
            }
        }
    }
    return maxString;
}
void InitRect(int colWidth, int rowHeight, RECT* rect)
{
    rect->left = colWidth + BORDER;
    rect->right = rect->left + colWidth - 2*BORDER;
    rect->top = rowHeight + BORDER;
    rect->bottom = rect->top + rowHeight - 2*BORDER;
}

TablePainter::TablePainter(HDC hdc) {
    this->_colCount = -1;    
    this->_rowCount = -1;
    this->hdc = hdc;
}
TablePainter::TablePainter(HDC hdc,Strings content) {
    this->_colCount = content[0].size();
    this->_rowCount = content.size();
    this->content = content;              
    this->hdc = hdc;
    this->maxString=GetMaxString(content);
}
TablePainter::~TablePainter() {
    this->content.clear();
    DeleteDC(this->hdc); 
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
    if (colWidth > MIN_WIDTH) {
        HDC hmemDC = CreateCompatibleDC(hdc);
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
    rect.left = colWidth * j + BORDER;
    rect.right = rect.left + colWidth - 2* BORDER;
    rect.top = rowHeight * i + BORDER;
    rect.bottom = rect.top + rowHeight - 2* BORDER;
    DrawText(hdc, StringToLPCWSTR(content[i][j]), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORDBREAK);
}