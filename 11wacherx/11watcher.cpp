// 11watcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <string>
using std::wstring;
#include <conio.h>
#include "11watcher.h"


wstring GetWinTitle(HWND hwnd)
{
    DWORD sz=GetWindowTextLength(hwnd);
    wstring ret;
    WCHAR *buf=new WCHAR[sz+1];
    GetWindowText(hwnd,buf,sz+1);
    ret=buf;
    delete buf;
    return ret;
}

HWND FindWindowByPrefix(HWND parent,wstring titlepre)
{
    for(HWND hwnd=FindWindowEx(parent,NULL,NULL,NULL);hwnd;hwnd=FindWindowEx(parent,hwnd,NULL,NULL))
    {
        wstring title=GetWinTitle(hwnd);
        if(title.find(titlepre)!=wstring::npos)
        {
            return hwnd;
        }
    }

    return NULL;
}

enum state run()
{
    HWND hwnd=FindWindowByPrefix(GetDesktopWindow(),L"Mimya-");
    if(hwnd)
        {
        HWND tiphwnd=FindWindowByPrefix(hwnd,L"房主需要在");
        if(tiphwnd)
            {
            if(IsWindowVisible(tiphwnd))
                {
                return ready;
                }
            else
                {
                return preparing;
                }
            }

        }
    return nohost;
}

