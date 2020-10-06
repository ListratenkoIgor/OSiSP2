
#include "Figure.h"

AuthorRect::AuthorRect(int height, int width) {
	this->height = height;
	this->width = width;
	brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
}

void AuthorRect::Draw(HDC hdc, int x, int y) {

	RECT r; //объ€вл€ем экзмепл€р структуры RECT - координаты пр€моугольника.
	r.left = x; //левый верхний угол
	r.top = y;
	r.right = x + height; //правый нижний
	r.bottom = y + width;
	//«аполн€ем пр€моугольник
	FillRect(hdc, &r, brush);
}

AuthorEllipse::AuthorEllipse(int a, int b) {
	this->a = a;
	this->b = b;									 
	brush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
}
void AuthorEllipse::Draw(HDC hdc, int x, int y) {

	RECT r; //объ€вл€ем экзмепл€р структуры RECT - координаты пр€моугольника.
	r.left = x; //левый верхний угол
	r.top = y;
	r.right = x + 2 * a; //правый нижний
	r.bottom = y + 2 * b;
	Ellipse(hdc, r.left, r.top, r.right, r.bottom);
}
void AuthorEllipse::DrawFromCenter(HDC hdc, int centerX, int centerY) {
	RECT r; //объ€вл€ем экзмепл€р структуры RECT - координаты пр€моугольника.
	r.left = centerX - a; //левый верхний угол
	r.top = centerY - b;
	r.right = centerX + a; //правый нижний
	r.bottom = centerY + b;
	Ellipse(hdc, r.left, r.top, r.right, r.bottom);
}