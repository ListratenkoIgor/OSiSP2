#include <windows.h>
#include "Figures.h"

void IFigure::Draw(HDC hdc,int x,int y) {


}
void IFigure::OnResize(RECT r, int* x, int* y) {
	if (*x < 0)
		*x = 0;
	if (*y < 0)
		*y = 0;
	if ((*x + this->width) > r.right) {
		*x = r.right - this->width;
	}
	if ((*y + this->height) > r.bottom) {
		*y = r.bottom - this->height;
	}								 								
}

void IFigure::Move(RECT r, int* x, int* y, int direction) {
	int newX = *x, newY = *y;
	switch (direction)
	{
	case USER_LEFT:
		if (CheckBound(r, newX -SHIFT_VALUE, newY)) {
			*x -= SHIFT_VALUE;
		}
		else {
			*x += BOARD_KICKBACK;
		}
		break;
	case USER_UP:
		if (CheckBound(r, newX , newY - SHIFT_VALUE)) {
			*y -= SHIFT_VALUE;
		}
		else {
			*y += BOARD_KICKBACK;
		}
		break;
	case USER_RIGHT:
		if (CheckBound(r, newX + SHIFT_VALUE, newY)) {
			*x += SHIFT_VALUE;
		}
		else {
			*x -= BOARD_KICKBACK;
		}
		break;
	case USER_DOWN:
		if (CheckBound(r, newX, newY + SHIFT_VALUE)) {
			*y += SHIFT_VALUE;
		}
		else {
			*y -= BOARD_KICKBACK;
		}
		break;
	default:
		break;
	}

}
bool IFigure::CheckBound(RECT r, int x, int y) {
	if ((x >= r.left) && (x + width <= r.right) && (y >= r.top) && (y + height <= r.bottom))
		return true;
	return false;
}

AuthorRect::AuthorRect(int width,int height) {
	this->height = height;
	this->width = width;
	pen = (HPEN)CreatePen(PS_DASHDOTDOT, 2, 0x0000FF00);
	brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
}
void AuthorRect::Draw(HDC hdc, int x, int y) {
	RECT r; 
	r.left = x; 
	r.top = y;
	r.right = x + height; 
	r.bottom = y + width;
	HGDIOBJ  hOld = SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	Rectangle(hdc, r.left, r.top, r.right, r.bottom);
	SelectObject(hdc, hOld);
}

AuthorEllipse::AuthorEllipse(int width, int height) {
	this->width = width;
	this->height = height;
	pen = (HPEN)CreatePen(PS_DOT, 2, 0x000000FF);
	brush = (HBRUSH)CreateSolidBrush(RGB(0, 255, 0));
}
void AuthorEllipse::Draw(HDC hdc, int x, int y) {
	RECT r; 
	r.left = x; 
	r.top = y;
	r.right = x + width; 
	r.bottom = y + height;
	HGDIOBJ  hOld = SelectObject(hdc, brush);
	SelectObject(hdc, pen);
	Ellipse(hdc, r.left, r.top, r.right, r.bottom); 
	SelectObject(hdc, hOld);
}

AuthorBitmap::AuthorBitmap(BITMAPINFOHEADER bmi, HBITMAP hBitmap) {
	this->hBitmap = hBitmap;
	width = bmi.biWidth;
	height = bmi.biHeight;
}
void AuthorBitmap::Draw(HDC hdc, int x, int y) {
	HDC hMemDC = CreateCompatibleDC(hdc);
	HGDIOBJ  hOld = SelectObject(hMemDC, hBitmap);
	TransparentBlt(hdc, x, y, width, height, hMemDC, 0, 0, width, height, 0x00FFFFFF);
	SelectObject(hMemDC, hOld);
	DeleteDC(hMemDC);
}

Bridge::Bridge(IFigure *figure) :_currentFigure(figure) {
	_currentFigure = figure;
}
void Bridge::SetInterface(IFigure *figure) {
	_currentFigure = figure;
}
void Bridge::Draw(HDC hdc, int x, int y) {
	_currentFigure->Draw(hdc, x, y);
}
void Bridge::Move(RECT r, int* x, int* y, int direction) {
	_currentFigure->Move(r, x, y,direction);
}
void Bridge::OnResize(RECT r, int* x, int* y) {
	_currentFigure->OnResize(r, x, y);
}