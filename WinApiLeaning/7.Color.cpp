#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ScrollProc(HWND, UINT, WPARAM, LPARAM);

int     idFocus;
WNDPROC OldScroll[3];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Colors1");
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
    wndclass.hbrBackground = CreateSolidBrush(0);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("This program requires Windows NT!"),
            szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName, TEXT("Color Scroll"),
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
    static COLORREF crPrim[3] = { RGB(255, 0, 0), RGB(0, 255, 0),
                                  RGB(0, 0, 255) };
    static HBRUSH  hBrush[3], hBrushStatic;
    static HWND    hwndScroll[3], hwndLabel[3], hwndValue[3], hwndRect;
    static int     color[3], cyChar;
    static RECT    rcColor;
    static const TCHAR* szColorLabel[] = { TEXT("Red"), TEXT("Green"),
                                      TEXT("Blue") };
    HINSTANCE      hInstance;
    int            i, cxClient, cyClient;
    TCHAR          szBuffer[10];

    switch (message)
    {
    case WM_CREATE:
        hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);

        // Create the white-rectangle window against which the 
        // scroll bars will be positioned. The child window ID is 9.

        hwndRect = CreateWindow(TEXT("static"), NULL,
            WS_CHILD | WS_VISIBLE | SS_WHITERECT,
            0, 0, 0, 0,
            hwnd, (HMENU)9, hInstance, NULL);

        for (i = 0; i < 3; i++)
        {
            // The three scroll bars have IDs 0, 1, and 2, with
            // scroll bar ranges from 0 through 255.

            hwndScroll[i] = CreateWindow(TEXT("scrollbar"), NULL,
                WS_CHILD | WS_VISIBLE |
                WS_TABSTOP | SBS_VERT,
                0, 0, 0, 0,
                hwnd, (HMENU)i, hInstance, NULL);

            SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
            SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

            // The three color-name labels have IDs 3, 4, and 5, 
            // and text strings "Red", "Green", and "Blue".

            hwndLabel[i] = CreateWindow(TEXT("static"), szColorLabel[i],
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                0, 0, 0, 0,
                hwnd, (HMENU)(i + 3),
                hInstance, NULL);

            // The three color-value text fields have IDs 6, 7, 
            // and 8, and initial text strings of "0".

            hwndValue[i] = CreateWindow(TEXT("static"), TEXT("0"),
                WS_CHILD | WS_VISIBLE | SS_CENTER,
                0, 0, 0, 0,
                hwnd, (HMENU)(i + 6),
                hInstance, NULL);

            //设置一个窗口子类
            OldScroll[i] = (WNDPROC)SetWindowLong(hwndScroll[i],
                GWL_WNDPROC, (LONG)ScrollProc);

            //创建三个画刷红绿蓝用来绘制滚动条
            hBrush[i] = CreateSolidBrush(crPrim[i]);
        }

        hBrushStatic = CreateSolidBrush(
            GetSysColor(COLOR_BTNHIGHLIGHT));

        cyChar = HIWORD(GetDialogBaseUnits());
        return 0;

    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);

        //右边显示颜色的矩形
        SetRect(&rcColor, cxClient / 2, 0, cxClient, cyClient);

        //Changes the position and dimensions of the specified window. 
        //For a top-level window, the position and dimensions are 
        //relative to the upper-left corner of the screen.
        //For a child window, they are relative to the upper-left 
        //corner of the parent window's client area.

        MoveWindow(hwndRect, 0, 0, cxClient / 2, cyClient, TRUE);

        //这里绘制了窗口控件嘛？
        for (i = 0; i < 3; i++)
        {
            MoveWindow(hwndScroll[i],
                (2 * i + 1) * cxClient / 14, 2 * cyChar,
                cxClient / 14, cyClient - 4 * cyChar, TRUE);

            MoveWindow(hwndLabel[i],
                (4 * i + 1) * cxClient / 28, cyChar / 2,
                cxClient / 7, cyChar, TRUE);

            MoveWindow(hwndValue[i],
                (4 * i + 1) * cxClient / 28,
                cyClient - 3 * cyChar / 2,
                cxClient / 7, cyChar, TRUE);
        }
        SetFocus(hwnd);
        return 0;

    case WM_SETFOCUS:
        SetFocus(hwndScroll[idFocus]);
        return 0;

    case WM_VSCROLL:
        //这个应该是窗口进程的id，对应于CreateWindow指定的id
        i = GetWindowLong((HWND)lParam, GWL_ID);

        switch (LOWORD(wParam))
        {
        case SB_PAGEDOWN:
            color[i] += 15;
            // fall through
        case SB_LINEDOWN:
            color[i] = min(255, color[i] + 1);
            break;

        case SB_PAGEUP:
            color[i] -= 15;
            // fall through
        case SB_LINEUP:
            color[i] = max(0, color[i] - 1);
            break;

        case SB_TOP:
            color[i] = 0;
            break;

        case SB_BOTTOM:
            color[i] = 255;
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            color[i] = HIWORD(wParam);
            break;

        default:
            break;
        }
        SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
        wsprintf(szBuffer, TEXT("%i"), color[i]);

        //把下面显示进度条数值的数字进行修改
        SetWindowText(hwndValue[i], szBuffer);


        //删除画笔
        DeleteObject((HBRUSH)
            SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)
                //RGB就是红绿蓝三个颜色混起来，这里把三个进度条的值放到这里然后画出来
                CreateSolidBrush(RGB(color[0], color[1], color[2]))));

        //使矩形区域无效，重绘无效区域
        InvalidateRect(hwnd, &rcColor, TRUE);
        return 0;

    
        //The WM_CTLCOLORSCROLLBAR message is sent to the parent window of 
        //a scroll bar control when the control is about to be drawn. 
        //By responding to this message, the parent window can use the display
        //context handle to set the background color of the scroll bar control.

        //所以返回一个背景色给窗口告诉父窗口绘制就以这个返回的颜色为背景色
    case WM_CTLCOLORSCROLLBAR:
        i = GetWindowLong((HWND)lParam, GWL_ID);
        return (LRESULT)hBrush[i];

        //和上面的WM_CTLCOLORSCROLLBAR 是一样的，只不过因为不是一个模块所以返回的处理消息是不一样的。
    case WM_CTLCOLORSTATIC:
        i = GetWindowLong((HWND)lParam, GWL_ID);

        if (i >= 3 && i <= 8)    // static text controls
        {
            SetTextColor((HDC)wParam, crPrim[i % 3]);
            SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
            return (LRESULT)hBrushStatic;
        }
        break;

    case WM_SYSCOLORCHANGE:
        DeleteObject(hBrushStatic);
        hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
        return 0;

    case WM_DESTROY:
        //释放画笔
        DeleteObject((HBRUSH)
            SetClassLong(hwnd, GCL_HBRBACKGROUND, (LONG)
                GetStockObject(WHITE_BRUSH)));

        for (i = 0; i < 3; i++)
            DeleteObject(hBrush[i]);

        DeleteObject(hBrushStatic);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
//子窗口过程用于使用Tab键切换窗口焦点
LRESULT CALLBACK ScrollProc(HWND hwnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    int id = GetWindowLong(hwnd, GWL_ID);

    switch (message)
    {
    case WM_KEYDOWN:
        if (wParam == VK_TAB)
            SetFocus(GetDlgItem(GetParent(hwnd),
            (id + (GetKeyState(VK_SHIFT) < 0 ? 2 : 1)) % 3));
        break;

    case WM_SETFOCUS:
        idFocus = id;
        break;
    }
    return CallWindowProc(OldScroll[id], hwnd, message, wParam, lParam);
}