/*-------------------------------------------------
   CHECKER1.C -- Mouse Hit-Test Demo Program No. 1
                 (c) Charles Petzold, 1998
  -------------------------------------------------*/

#include <windows.h>
#include <windowsx.h>
#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR  szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Checker1");
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

    hwnd = CreateWindow(szAppName, TEXT("Checker1 Mouse Hit-Test Demo"),
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //一个5x5的bool数组，标识某块区域是否被绘制
    static BOOL fState[DIVISIONS][DIVISIONS];
    //每块的长宽
    static int  cxBlock, cyBlock;
    HDC         hdc;
    int         x, y;
    PAINTSTRUCT ps;
    RECT        rect;

    switch (message)
    {
    case WM_SIZE:
        cxBlock = LOWORD(lParam) / DIVISIONS;
        cyBlock = HIWORD(lParam) / DIVISIONS;
        return 0;

    case WM_LBUTTONDOWN:
        //获得鼠标的xy坐标，这里直接int去整就不需要判断了，妙啊
        x = GET_X_LPARAM(lParam) / cxBlock;
        y = GET_Y_LPARAM(lParam) / cyBlock;

        if (x < DIVISIONS && y < DIVISIONS)
        {
            //异或运算符，0^0=1^1=0,1^0=0^1=1
            fState[x][y] ^= 1;

            rect.left = x * cxBlock;
            rect.top = y * cyBlock;
            rect.right = (x + 1) * cxBlock;
            rect.bottom = (y + 1) * cyBlock;

            //将对应区域的
            InvalidateRect(hwnd, &rect, FALSE);
        }
        else
            MessageBeep(0);
        return 0;

    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        //在这里加上        
        //cxBlock = LOWORD(lParam) / DIVISIONS;
        //cyBlock = HIWORD(lParam) / DIVISIONS;
        //这两句会使case WM_LBUTTONDOWN 中对xy赋值语句因除数为零报错
        for (x = 0; x < DIVISIONS; x++)
            for (y = 0; y < DIVISIONS; y++)
            {
                Rectangle(hdc, x * cxBlock, y * cyBlock,
                    (x + 1) * cxBlock, (y + 1) * cyBlock);

                if (fState[x][y])
                {
                    MoveToEx(hdc, x * cxBlock, y * cyBlock, NULL);
                    LineTo(hdc, (x + 1) * cxBlock, (y + 1) * cyBlock);
                    MoveToEx(hdc, x * cxBlock, (y + 1) * cyBlock, NULL);
                    LineTo(hdc, (x + 1) * cxBlock, y * cyBlock);
                }
            }
        EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}