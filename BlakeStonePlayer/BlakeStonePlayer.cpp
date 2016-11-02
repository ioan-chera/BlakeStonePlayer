// BlakeStonePlayer.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"

static const char gWindowName[] = "DOSBox 0.74, Cpu speed: max 100% cycles, Frameskip  0, Program:   BS_AOG";
//static const char gWindowName[] = "DOSBox";

static void hitEnter()
{
   INPUT input[2];
   memset(input, 0, sizeof(input));
   input[0].type = input[1].type = INPUT_KEYBOARD;
   input[0].ki.wVk = input[1].ki.wVk = VK_RETURN;
   input[1].ki.dwFlags = KEYEVENTF_KEYUP;
   if (!SendInput(1, input, sizeof(INPUT)))
   {
      fprintf(stderr, "SendInput failed %lu\n", GetLastError());
   }
   Sleep(100);
   if (!SendInput(1, input + 1, sizeof(INPUT)))
   {
      fprintf(stderr, "SendInput 2 failed %lu\n", GetLastError());
   }

}

int main()
{
   // http://stackoverflow.com/a/7292773
   HWND wnd = FindWindowA(nullptr, gWindowName);
   if (!wnd)
   {
      fprintf(stderr, "Window not found\n");
      return EXIT_FAILURE;
   }

   if (!SetForegroundWindow(wnd))
   {
      fprintf(stderr, "Failed focusing, %lu\n", GetLastError());
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

   if (!PrintWindow(wnd, dc, PW_CLIENTONLY))
   {
      return EXIT_FAILURE;
   }

   LONG width = rc.right - rc.left;
   LONG height = rc.bottom - rc.top;

   BITMAPINFO binfo;
   memset(&binfo, 0, sizeof(binfo));
   binfo.bmiHeader.biSize = sizeof(binfo.bmiHeader);
   binfo.bmiHeader.biWidth = width;
   binfo.bmiHeader.biHeight = height;
   binfo.bmiHeader.biPlanes = 1;
   binfo.bmiHeader.biBitCount = 32;
   binfo.bmiHeader.biCompression = BI_RGB;
   
   int result = GetDIBits(dc, bmp, 0, height, nullptr, &binfo, DIB_RGB_COLORS);
   if (!result)
   {
      return EXIT_FAILURE;
   }
   printf("Size: %lu\n", binfo.bmiHeader.biSizeImage);
   COLORREF *pixels = new COLORREF[binfo.bmiHeader.biSizeImage / sizeof(COLORREF)];
   result = GetDIBits(dc, bmp, 0, height, pixels, &binfo, DIB_RGB_COLORS);
   if (!result)
   {
      return EXIT_FAILURE;
   }

   unsigned totr = 0, totg = 0, totb = 0;
   COLORREF color;
   for (LONG i = 0; i < rc.right; ++i)
   {
      color = pixels[0 * width + i];
      if (color == CLR_INVALID)
      {
         puts("Bad color");
         return EXIT_FAILURE;
      }
      else
      {
         totb += color & 0xff;
         totg += (color & 0xff00) >> 8;
         totr += (color & 0xff0000) >> 16;
      }
   }
   printf("Mean color: %u %u %u\n", totr / width, totg / width, totb / width);

   INPUT input;
   memset(&input, 0, sizeof(input));
   input.type = INPUT_KEYBOARD;
   input.ki.wVk = VK_RETURN;

   hitEnter();
   Sleep(1000);
   hitEnter();
   Sleep(1000);
   hitEnter();
   Sleep(1000);
   hitEnter();
   Sleep(5000);
   hitEnter();
   
//   OpenClipboard(nullptr);
//   EmptyClipboard();
//   SetClipboardData(CF_BITMAP, bmp);
//   CloseClipboard();

   delete[] pixels;
   DeleteDC(dc);
   DeleteObject(bmp);
   ReleaseDC(nullptr, dcScreen);

    return EXIT_SUCCESS;
}

