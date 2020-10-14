#include <windows.h>
#pragma comment(lib, "Msimg32.lib")
#define USER_LEFT      0x00000000
#define USER_UP        0x00000001
#define USER_RIGHT     0x00000002
#define USER_DOWN      0x00000003
#define SHIFT_VALUE    0x00000005
#define BOARD_KICKBACK 0x0000000E

class IFigure {
protected:
	int height;
	int width;
	HBRUSH brush;
	HPEN pen;
public:
	virtual void Draw(HDC, int, int);
	virtual void Move(RECT, int*, int*, int);
	void Resize(RECT, int*, int*);
private:
	virtual bool CheckBound(RECT, int, int);
};


class AuthorRect :public IFigure
{		  
public:
	AuthorRect(int, int);
	virtual void Draw(HDC, int, int) override;
};

class AuthorEllipse :public IFigure
{

public:
	AuthorEllipse(int, int);
	virtual void Draw(HDC, int, int) override;
};
class AuthorBitmap :public IFigure
{				  
private:
	HBITMAP hBitmap;
public:
	AuthorBitmap(BITMAPINFOHEADER, HBITMAP);
	virtual void Draw(HDC, int, int) override;
};

class Bridge {
protected:
	IFigure *_currentFigure;
public:
	Bridge(IFigure *figure);
	void SetInterface(IFigure *figure);
	void Draw(HDC, int, int);
	void Move(RECT, int*, int*, int);
	void Resize(RECT, int*, int*);
};
/**/