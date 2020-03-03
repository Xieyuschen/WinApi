// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"

#define MAX_LOADSTRING 100

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
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    //������Ϣ����Դ�����
    //GetMessage��msg��ʼ����������Ҫmsg�ĵ�ַ��д���ݣ�
    //��Windows��Ϣ���е���һ����Ϣ����msg��ֵ�����Ծ�ֻ��ѭ���ⶨ����MSG�ṹ
    //msg��ǰ׺
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // msg�еĳ�Աmessage(int���͵�),ÿ����ϢWinUser.hͷ�ļ��ж������� WM(WindowsMessage)Ϊǰ׺�ı�ʶ��
        //msg�е�pt��POINT���ͣ���������Ϣ�������ʱ���������

        //���GetMessage()����Ϣ���н��յ�����ϢΪ WM_EXIT,�ͷ���0
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: ע�ᴰ����
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    //Windowsˮƽ��ֱ�ߴ�仯��ʱǿ���ػ�
    wcex.style          = CS_HREDRAW | CS_VREDRAW;

    //ָ���˴�����Ĵ��ڹ���
    wcex.lpfnWndProc    = WndProc;

    //�������ռ�
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;

    //Ӧ�ó����ʵ�����
    wcex.hInstance      = hInstance;

    //����ͼ��ľ����LoadIconde
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    //�������ľ�����ڶ����������������ʽ
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);

    //��ˢ�ľ������ʾ���ɫ��ģʽ
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);

    //���ô�����Ĳ˵�
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);

    //���ô����������
    wcex.lpszClassName  = szWindowClass;

    //Ϊ�ô�������һ��ͼ��
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //����һ��ָ��������ָ��
    //ʵ����ϵͳ��������ע���õĺ���
    //RegisterClassW && RegisterClassA,ʹ����һ��ȡ���ڴ��ݸ����ڵ���Ϣ��ASCII����Unicode

    //��win98�±���ʹ��Unicode���������RegisterClassWֻ�Ǽ򵥷���0��û�о���ʵ��
    //����Ҫ����һ�¼�飬��Ȼ��Windos NT����û�����
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

   //����һ��ָ�򴴽����ڵľ��
   HWND hWnd = CreateWindowW(
       szWindowClass,//���������ƣ�����ڴ�������������ʱ���Ѿ�ȡ�����֣��ô˷�ʽ�봰���ཨ����ϵ
       __T("SetSrcol"),//���ڱ���
       WS_OVERLAPPEDWINDOW|WS_VSCROLL,//���ڷ��
       CW_USEDEFAULT,
       0, 
       CW_USEDEFAULT,
       0, 
       nullptr,//�����ھ��
       nullptr, //����ʵ�����
       hInstance, //����ʵ�����
       nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   //�ڶ���������WinMain���ܵĵ���������
   //������������Ļ�ĳ�ʼ��ʾ��ʽ������||��С��||��󻯣�
   //   ShowWindow(hWnd, SW_SHOWMAXIMIZED),���г���������󻯵�
   //   ShowWindow(hWnd, SW_SHOWMINIMIZED),��С��
   //������ӵ������ʱ���û�ѡ���ƫ�û�ͨ��nCMdShow����
   //����ʲôShowWindow�Ͱ�ʲô��ʽ����ʾ����
   ShowWindow(hWnd, nCmdShow);

   //�ػ洰�ڿͻ���
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
    static int cxChar, cyChar ,cxClient, cyClient,iVsrollPos;
    static int iVertPos,iHorzPos;
    SCROLLINFO si;
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
    case WM_VSCROLL:
    {
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(hWnd, SB_VERT, &si);
        iVertPos = si.nPos;
        switch(LOWORD(wParam)) 
        {
        case SB_TOP:
            si.nPos = si.nMin;
            break;
        case SB_BOTTOM:
            si.nPos = si.nMax;
            break;
        case SB_LINEUP:
            --si.nPos;
            break;
        case SB_LINEDOWN:
            ++si.nPos;
            break;
        case SB_PAGEUP:
            si.nPos -=si.nPage;
            break;
        case SB_PAGEDOWN:
            si.nPos +=si.nPage;
            break;
        case SB_THUMBPOSITION:
            si.nPos = si.nTrackPos;
            break;
        default:
            break;
        }
        si.fMask = SIF_POS;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        GetScrollInfo(hWnd, SB_VERT, &si);
        if (si.nPos != iVertPos) {
            ScrollWindow(hWnd, 0, cyChar * (iVertPos-si.nPos), NULL, NULL);
            UpdateWindow(hWnd);
        }
        break;
    }

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            const TCHAR* p1 = TEXT("1234567890");
            si.cbSize = sizeof(si);
            si.nPos = SIF_POS;
            GetScrollInfo(hWnd, SB_VERT, &si);
            iVertPos = si.nPos;
         
            auto iPaintEnd = min(69, iVertPos + ps.rcPaint.bottom / cyChar);
            auto iPaintBeg = max( 0,iVertPos+ ps.rcPaint.top / cyChar);
            for (int i = iPaintBeg; i <iPaintEnd; i++) {

                int y = cyChar*(i-iVertPos);
                TextOut(hdc, 0, y, p1+i%10,1);
            }


            TEXTMETRIC tm;
            GetTextMetrics(hdc,&tm);
            
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        //LOWDRN����WORD,0~0XFFFF
        cyClient = HIWORD(lParam);
        si.cbSize = sizeof(si);
        si.fMask = SIF_RANGE | SIF_PAGE;
        si.nMin = 0;
        si.nMax = 70;
        si.nPage = cyClient / cyChar;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_HOME:
            SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);
            break;
        case VK_END:
            SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
            break;
        case VK_PRIOR:
            SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
            break;
        case VK_NEXT:
            SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
            break;  
        case VK_UP:
            SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
            break;
        case VK_DOWN:
            SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
            break;
        default: 
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        HDC hdc = GetDC(hWnd);
        TEXTMETRIC tm;
        GetTextMetrics(hdc,&tm);
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
