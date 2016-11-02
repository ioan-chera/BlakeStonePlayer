// BlakeStonePlayer.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"

static const char gWindowName[] = "DOSBox 0.74, Cpu speed: max 100% cycles, Frameskip  0, Program:   BS_AOG";
//static const char gWindowName[] = "DOSBox";

int main()
{
   // http://stackoverflow.com/a/7292773
   HWND wnd = FindWindowA(nullptr, gWindowName);
   if (!wnd)
   {
      fprintf(stderr, "Window not found\n");
      return EXIT_FAILURE;
   }

   RECT rc;
   if (!GetClientRect(wnd, &rc))
   {
      fprintf(stderr, "GetClientRect failed with %lu\n", GetLastError());
      return EXIT_FAILURE;
   }

   HDC dcScreen = GetDC(nullptr);
   HDC dc = CreateCompatibleDC(dcScreen);
   HBITMAP bmp = CreateCompatibleBitmap(dcScreen, rc.right - rc.left, rc.bottom - rc.top);
   SelectObject(dc, bmp);

   PrintWindow(wnd, dc, PW_CLIENTONLY);
   
   OpenClipboard(nullptr);
   EmptyClipboard();
   SetClipboardData(CF_BITMAP, bmp);
   CloseClipboard();

   DeleteDC(dc);
   DeleteObject(bmp);
   ReleaseDC(nullptr, dcScreen);

    return EXIT_SUCCESS;
}

