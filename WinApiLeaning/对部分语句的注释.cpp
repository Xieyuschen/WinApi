// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"
#include <math.h>
#define MAX_LOADSTRING 100
#define NUM 1000
#define TWOPI 2*3.14159
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    //所以消息的来源是哪里？
    //GetMessage给msg初始化（所以需要msg的地址来写内容）
    //用Windows消息队列的下一条消息来给msg赋值，所以就只在循环外定义了MSG结构
    //msg的前缀
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // msg中的成员message(int类型的),每条消息WinUser.h头文件中定义了以 WM(WindowsMessage)为前缀的标识符
        //msg中的pt（POINT类型），表明消息进入队列时的鼠标坐标

        //如果GetMessage()从消息队列接收到的消息为 WM_EXIT,就返回0
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: 注册窗口类
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    //Windows水平竖直尺寸变化的时强行重绘
    wcex.style = CS_HREDRAW | CS_VREDRAW;

    //指定此窗口类的窗口过程
    wcex.lpfnWndProc = WndProc;

    //分配额外空间
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;

    //应用程序的实例句柄
    wcex.hInstance = hInstance;

    //设置图标的句柄，LoadIconde
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    //返回鼠标的句柄，第二个参数设置鼠标样式
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

    //画刷的句柄，表示填充色的模式
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    //设置窗口类的菜单
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);

    //设置窗口类的名字
    wcex.lpszClassName = szWindowClass;

    //为该窗口设置一个图标
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //接受一个指向类对象的指针
    //实际上系统存在两个注册用的函数
    //RegisterClassW && RegisterClassA,使用哪一个取决于传递给窗口的消息是ASCII还是Unicode

    //在win98下编译使用Unicode这种情况，RegisterClassW只是简单返回0，没有具体实现
    //所以要进行一下检查，当然在Windos NT上是没问题的
    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    //返回一个指向创建窗口的句柄
    HWND hWnd = CreateWindowW(
        szWindowClass,//窗口类名称，这个在创建窗口类对象的时候已经取了名字，用此方式与窗口类建立联系
        __T("Graph"),//窗口标题
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        nullptr,//父窗口句柄
        nullptr, //程序实例句柄
        hInstance, //程序实例句柄
        nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);

    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int cxChar, cyChar, cxClient, cyClient, iVsrollPos;
    static int iVertPos, iHorzPos;
    SCROLLINFO si;
    TCHAR szBuffer[10];
    POINT apt[NUM];
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
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
        HBRUSH hBrush= CreateSolidBrush(RGB(0, 0, 255));
        RECT rect1{ 0,0,40,40 }, rect2{ 120,0,160,40 }, rect3{240,0,280,40};
        FillRect(hdc, &rect1, hBrush);
        FrameRect(hdc, &rect2, hBrush);
        InvertRect(hdc, &rect1);
        InvertRect(hdc, &rect2);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZE:
        //LOWDRN返回WORD,0~0XFFFF
        cyClient = HIWORD(lParam);
        cxClient = LOWORD(lParam);
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 70;
        si.nPage = cyClient / cyChar;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSCHAR:
        break;
    default:
        HDC hdc = GetDC(hWnd);
        TEXTMETRIC tm;
        GetTextMetrics(hdc, &tm);
        cxChar = tm.tmAveCharWidth;
        cyChar = tm.tmHeight + tm.tmExternalLeading;
        ReleaseDC(hWnd, hdc);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
