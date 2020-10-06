// OSiSP-1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Figures.h"
#include "OSiSP-1.h"

#define MAX_LOADSTRING 100
#define USER_HEIGHT 100
#define USER_WIDTH 100
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
Bridge* bridge;									// паттерн "Мост" для общей реализации отрисовки фигур через интерфейс
int x = 0;int y = 0;							// начальные координаты
BITMAPINFOHEADER bmi;
HBITMAP hMemBitmap;

int g_windowWidth;
int g_windowHeight;
	

void OnCreate(HWND, WPARAM, LPARAM);
												
// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// TODO: Разместите код здесь.
 	bridge = new Bridge((IFigure*)(new AuthorRect(100, 100)));
	// Инициализация глобальных строк
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_OSISP1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	// Выполнить инициализацию приложения:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISP1));
	MSG msg;

	// Цикл основного сообщения:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_OSISP1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	int fwKeys ;
	int zDelta ;
	switch (message)
	{

	case WM_KEYDOWN: {
		//RECT rect;
		//GetClientRect(hWnd, &rect);
		switch (wParam)
		{
		case VK_LEFT:
			bridge->Move(rect, &x, &y, USER_LEFT);
			break;
		case VK_UP:			
			bridge->Move(rect, &x, &y, USER_UP);
			break;
		case VK_RIGHT:			
			bridge->Move(rect, &x, &y, USER_RIGHT);
			break;
		case VK_DOWN:
			bridge->Move(rect, &x, &y, USER_DOWN);
			break;
		case 'b':
		case 'B':
			bridge->SetInterface((IFigure*)new AuthorBitmap(bmi, hMemBitmap));
			break;
		case 'r':
		case 'R':
			bridge->SetInterface((IFigure*)new AuthorRect(USER_WIDTH, USER_HEIGHT));
			break;
		case 'e':
		case 'E':
			bridge->SetInterface((IFigure*)new AuthorEllipse(USER_WIDTH, USER_HEIGHT));
			break;

		default:
			break;
		}
		//InvalidateRect(hWnd,NULL,false);
		RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT r;
		GetClientRect(hWnd,&r);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		HDC hMemDC = CreateCompatibleDC(hdc);
		HBITMAP hBM = CreateCompatibleBitmap(hdc, r.right, r.bottom);
		HANDLE hOld = SelectObject(hMemDC, hBM);
		
		FillRect(hMemDC, &r, (HBRUSH)CreateSolidBrush(RGB(0,0,255)));
		bridge->Draw(hMemDC, x, y);
		BitBlt(hdc, 0, 0, r.right , r.bottom , hMemDC, 0, 0, SRCCOPY);

		SelectObject(hMemDC, hOld);
		DeleteObject(hBM);
		DeleteDC(hMemDC);

		
		EndPaint(hWnd, &ps);
		
	}
	break;
	case WM_MOUSEWHEEL:
		fwKeys = GET_KEYSTATE_WPARAM(wParam);
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		switch (fwKeys) {
			case MK_SHIFT:
				if (zDelta > 0) {
					bridge->Move(rect, &x, &y, USER_LEFT);
				}
				else {
					bridge->Move(rect, &x, &y, USER_RIGHT);
				}
				break;
			default:
				if (zDelta > 0) {
					bridge->Move(rect, &x, &y, USER_UP);
				}
				else {
					bridge->Move(rect, &x, &y, USER_DOWN);
				}
				break;
		}
		RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW | RDW_INVALIDATE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		g_windowWidth = LOWORD(lParam);
		g_windowHeight = HIWORD(lParam);
		break;
	case WM_CREATE:
		OnCreate(hWnd, wParam, lParam);
		break;
	case WM_ERASEBKGND:
		//InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam) {

	BYTE* pData = NULL;
	HDC hdc = GetDC(hWnd);
	//L"c:/imagenew.bmp"
	HANDLE hFile = CreateFile(L"d:\\3.bmp", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwSize;
		SetFilePointer(hFile, sizeof(BITMAPFILEHEADER), NULL, FILE_BEGIN);
		ReadFile(hFile, &bmi, sizeof(bmi), &dwSize, NULL);
		int iAlign = ((bmi.biWidth * 3 + 3) / 4) * 4;
		pData = new BYTE[iAlign * bmi.biHeight * 3];
		ReadFile(hFile, pData, iAlign * bmi.biHeight * 3, &dwSize, NULL);	
		HDC hMemDC = CreateCompatibleDC(hdc);
		hMemBitmap = CreateCompatibleBitmap(hdc, bmi.biWidth, bmi.biHeight);
		HGDIOBJ  hOld = SelectObject(hMemDC, hMemBitmap);
		SetDIBitsToDevice(hMemDC, 0, 0, bmi.biWidth, bmi.biHeight, 0, 0, 0, bmi.biHeight, pData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
		SelectObject(hMemDC, hOld);
		CloseHandle(hFile);
	}
	else {
		MessageBox(NULL, L"Error in loading image", L"warning", MB_ICONERROR);
	}
	

}
