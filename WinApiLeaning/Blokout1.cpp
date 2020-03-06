/*-----------------------------------------
   BLOKOUT1.C -- Mouse Button Demo Program
                 (c) Charles Petzold, 1998
  -----------------------------------------*/

#include <windows.h>
#include <windowsx.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("BlokOut1");
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;

    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Mouse Button Demo"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

void DrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd)
{
    HDC hdc;

    hdc = GetDC(hwnd);

    SetROP2(hdc, R2_NOT);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);

    ReleaseDC(hwnd, hdc);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL  fBlocking, fValidBox;
    static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_LBUTTONDOWN:
        ptBeg.x = ptEnd.x = GET_X_LPARAM(lParam);
        ptBeg.y = ptEnd.y = GET_Y_LPARAM(lParam);

        DrawBoxOutline(hwnd, ptBeg, ptEnd);

        SetCursor(LoadCursor(NULL, IDC_CROSS));

        fBlocking = TRUE;
        return 0;

    case WM_MOUSEMOVE:
        if (fBlocking)
        {
            //这里两次调用DrawBoxline()的目的为绘制新的线，去掉原来的线
            SetCursor(LoadCursor(NULL, IDC_CROSS));
            
            //去掉旧的线，因为此时ptEnd还没有重新赋值
            DrawBoxOutline(hwnd, ptBeg, ptEnd);
            ptEnd.x = GET_X_LPARAM(lParam);
            ptEnd.y = GET_Y_LPARAM(lParam);

            //画出新的线
            DrawBoxOutline(hwnd, ptBeg, ptEnd);
        }
        return 0;

    case WM_LBUTTONUP:
        if (fBlocking)
        {
            DrawBoxOutline(hwnd, ptBeg, ptEnd);

            ptBoxBeg = ptBeg;
            ptBoxEnd.x = GET_X_LPARAM(lParam);
            ptBoxEnd.y = GET_Y_LPARAM(lParam);
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            fBlocking = FALSE;
            fValidBox = TRUE;

            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_CHAR:
        if (fBlocking & (wParam == '\x1B'))     // i.e., Escape
        {
            DrawBoxOutline(hwnd, ptBeg, ptEnd);

            SetCursor(LoadCursor(NULL, IDC_ARROW));

            fBlocking = FALSE;
        }
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);

        if (fValidBox)
        {
            //Paint环节里面将选中的方框涂黑
            SelectObject(hdc, GetStockObject(BLACK_BRUSH));
            Rectangle(hdc, ptBoxBeg.x, ptBoxBeg.y,
                ptBoxEnd.x, ptBoxEnd.y);
        }

        if (fBlocking)
        {
            
            SetROP2(hdc, R2_NOT);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
        }


        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}