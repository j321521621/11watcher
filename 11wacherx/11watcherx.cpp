// 11wacherx.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "11watcherx.h"
#include "11watcher.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


HICON hIconRed;
HICON hIconYellow;  
HICON hIconGrey;
HICON hIconGreen;
bool isfinding;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    ::CreateMutex(NULL,FALSE,L"[Guid(23C1895A-E971-4AF7-8F51-74E39B160DCA)]");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
        return -1;
        }

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY11WACHERX, szWindowClass, MAX_LOADSTRING);
    hIconRed = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_SMALL));
    hIconYellow = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YELLOW));  
    hIconGrey = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GREY));  
    hIconGreen = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GREEN));
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY11WACHERX));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY11WACHERX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST,szWindowClass, szTitle, WS_POPUP,
      200, 0, GetSystemMetrics(SM_CXSCREEN)-400, 60, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }


   SetTimer(hWnd,0,200,0);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

void ChangeNotifyIcon(state st,HWND hWnd, DWORD message)
    {
    NOTIFYICONDATA nid;
    nid.cbSize = sizeof(nid);  
    nid.hWnd = hWnd;  
    nid.uID = 0;  
    if(message == NIM_DELETE)
        {
        Shell_NotifyIcon(NIM_DELETE, &nid);  
        return;
        }
    if(st==nohost)
        {
        nid.hIcon = hIconGreen;  
        lstrcpy(nid.szTip, L"尚未建主");  
        }
    else if(st==ready)
        {
        nid.hIcon = hIconRed;  
        lstrcpy(nid.szTip, L"已准备完毕");  
        }
    else if(st==preparing)
        {
        nid.hIcon = hIconYellow;  
        lstrcpy(nid.szTip, L"已建主");  
        }
    if(message == NIM_ADD)
        {
        nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;  
        nid.uCallbackMessage = WM_USER;
        Shell_NotifyIcon(NIM_ADD, &nid);  
        }
    else if(message == NIM_MODIFY)
        {
        nid.uFlags = NIF_ICON | NIF_TIP;  
        Shell_NotifyIcon(NIM_MODIFY, &nid); 
        }
    }

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
    case WM_CREATE:
        ChangeNotifyIcon(run(),hWnd,NIM_ADD);
        break;
    case WM_TIMER:
        if(wParam==0)
            {
            state st = run();
            ChangeNotifyIcon(st,hWnd,NIM_MODIFY);
            if(st==ready )
                {
                if(!isfinding)
                    {
                    isfinding=true;
                    SetTimer(hWnd,1,1000,0);
                    }
                }
            else
                {
                isfinding=false;
                KillTimer(hWnd,1);
                }
            }
        if(wParam==1)
            {
            ShowWindow(hWnd,SW_SHOW);
            UpdateWindow(hWnd);
            Beep( 550, 300 );
            ShowWindow(hWnd,SW_HIDE);
            UpdateWindow(hWnd);
            }
        break;
    case WM_USER:
        if (lParam == WM_LBUTTONDBLCLK)
            {
            DestroyWindow(hWnd);
            }
        break;
    case WM_PAINT:
        {
        PAINTSTRUCT ps;
        BeginPaint(hWnd,&ps);
        RECT rc;
        GetClientRect(hWnd,&rc);
        HBRUSH brush = CreateSolidBrush(RGB(255,0,0));
        FillRect(ps.hdc,&rc,brush);
        DeleteObject(brush);
        EndPaint(hWnd,&ps);
        }
        break;
    case WM_DESTROY:
        ChangeNotifyIcon(ready,hWnd,NIM_DELETE);
        PostQuitMessage(0);
        break;
	default:
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
