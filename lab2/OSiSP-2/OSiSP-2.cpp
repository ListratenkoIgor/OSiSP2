// OSiSP-2.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "OSiSP-2.h"
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

TablePainter* painter;
MemoryWatcher* watcher;
Strings content;
int SystemConfigurate();
void OnCreate(HWND, WPARAM, LPARAM);
wstring OpenFile(HWND);


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
    try
    {
        //watcher = new MemoryWatcher("e:/MemoryLog.txt");
        SystemConfigurate();
        //watcher->WriteMemory(true, "start");
    }
    catch (const std::exception&)
    {
        MessageBox(NULL, L"Не найден файл логгирования", L"warning", MB_ICONERROR);
        return 1;
    }

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_OSISP2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISP2));
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
    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_OSISP2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
wstring OpenFile(HWND hWnd) {
    OPENFILENAME ofn;
    WCHAR szFile[256] = { 0 };
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("All files\0*.*\0Text\0*.TXT\0");
    ofn.nFilterIndex = 2;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn))
    {
        wstring str(ofn.lpstrFile);
        return str;
    }
    else {
        wstring str(ofn.lpstrFile);
        return str;
    }
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
    switch (message)
    {
    case WM_COMMAND:
        {
            wstring FilePath;   
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDC_OPENFILE_DIALOG:
                FilePath = OpenFile(hWnd);
                if (!FilePath.empty()) {
                    FileReader* reader = new FileReader();
                    content = reader->GetContentFromFile((std::string (FilePath.begin(), FilePath.end())));
                    delete reader;
                    if (!content.empty()) {                     
                        if (painter != nullptr) {
                            painter->SetContent(content);
                        }
                        else {
                            HDC hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
                            painter = new TablePainter(hdc, content);
                        }
                        RedrawWindow(hWnd, NULL, NULL, RDW_UPDATENOW| RDW_ERASENOW | RDW_INVALIDATE);
                    }                    
                }
                else
                {
                    if (watcher != NULL)
                        watcher->WriteMemory(true, "Close Dialog Cancel");
                }
                break;
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
            RECT rect;
            GetClientRect(hWnd, &rect);                                
            if (painter != nullptr) {
                painter->DrawTable(hdc,rect.right,rect.bottom);
            }
            else {
                MessageBox(NULL, L"Таблица строк не найдена,пожалуйста выберите файл", L"warning", MB_ICONERROR);
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CREATE:
        OnCreate(hWnd,wParam,lParam);
        break;
    case WM_SIZE:   
        break;
    case WM_DESTROY:
        delete painter;
  
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    FileReader* reader=new FileReader();
    content = reader->GetContentFromFile("e:/context3.txt");
    if (!content.empty()) {
        HDC hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        painter = new TablePainter(hdc,content);
    }
    delete reader;
}
int SystemConfigurate() {
    DWORD ErrorCode = GetLastError();
    SIZE_T MinimumWorkingSetSize;
    SIZE_T MaximumWorkingSetSize;
    bool success = true;
    HANDLE handle = GetCurrentProcess();
    try {
        SYSTEM_INFO SystemInfo;
        GetSystemInfo(&SystemInfo);
        MinimumWorkingSetSize = SystemInfo.dwPageSize * 1024;
        MaximumWorkingSetSize = SystemInfo.dwPageSize * 4096;
        success = SetProcessWorkingSetSizeEx(
            handle,
            MinimumWorkingSetSize,
            MaximumWorkingSetSize,
            QUOTA_LIMITS_HARDWS_MAX_ENABLE
        );
        ErrorCode = GetLastError();
        return 0;
    }
    catch (...) {
        MinimumWorkingSetSize = 81920;
        MaximumWorkingSetSize = 204800;
        SetProcessWorkingSetSizeEx(
            handle,
            MinimumWorkingSetSize,
            MaximumWorkingSetSize,
            QUOTA_LIMITS_HARDWS_MAX_ENABLE
        );
        return -1;
    }
}

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
