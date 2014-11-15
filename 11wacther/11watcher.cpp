// 11watcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <string>
using std::wstring;
#include <conio.h>


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


enum state
{
    nohost,
    error,
    preparing,
    ready
};

enum state run()
{
    HWND hwnd=FindWindowByPrefix(GetDesktopWindow(),L"Mimya-");
    if(!hwnd)
    {
        return nohost;
    }
    HWND tiphwnd=FindWindowByPrefix(hwnd,L"房主需要在");
    if(!tiphwnd)
    {
        return error;
    }

    if(IsWindowVisible(tiphwnd))
        {
        return ready;
        }

    return preparing;
}

int _tmain(int argc, _TCHAR* argv[])
{
    enum state pre=nohost;
    printf("尚未建主");
    while(true)
    {
        enum state now=run();
        if(now!=pre)
        {
            system("cls");
            pre=now;
            if(now==nohost)
            {
                printf("尚未建主");
            }
            if(now==error)
            {
                printf("错误,找不到static窗口");
            }
            if(now==preparing)
            {
                printf("监控中...");
            }
            if(now==ready)
            {
                printf("人满速开!!!");
            }
        }

        if(now==ready)
        {
            Beep( 550, 300 );
            Sleep(700);
        }
        else
        {
            Sleep(1000);
        }
    }
	return 0;
}

