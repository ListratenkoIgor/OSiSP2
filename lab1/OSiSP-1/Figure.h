#pragma once
#include <windows.h>

class IFigure {
public:
	HBRUSH brush;
	virtual void Draw(HDC , int , int );
};


class AuthorRect:public IFigure
{

public:
	int height;
	int width;
	AuthorRect(int ,int );
	virtual void Draw(HDC , int , int ) override;
};

class AuthorEllipse :public IFigure
{
public:
	int a;
	int b;
	AuthorEllipse(int , int );
	virtual void Draw(HDC , int , int) override;
	virtual void DrawFromCenter(HDC , int , int );
private:

};
