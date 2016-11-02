#include <Windows.h>

#include "GameWindow.h"

//
// Opens a game window. Returns false if it doesn't exist or other error
//
bool GameWindow::open(const char *name)
{
   mTitle = name;
   mWnd = FindWindowA(nullptr, name);
   if (!mWnd)
   {
      fprintf(stderr, "Window not found\n");
      return false;
   }
   if (!SetForegroundWindow(mWnd))
   {
      fprintf(stderr, "Failed focusing, %lu\n", GetLastError());
      return false;
   }

   if (!GetClientRect(mWnd, &mRect))
   {
      fprintf(stderr, "GetClientRect failed with %lu\n", GetLastError());
      return false;
   }

   mBInfo.bmiHeader.biWidth = mRect.right - mRect.left;
   mBInfo.bmiHeader.biHeight = mRect.bottom - mRect.top;

   return true;
}

//
// Takes a screenshot into memory
//
bool GameWindow::screenshot()
{
   if (!mDcScreen)
      mDcScreen = GetDC(nullptr);
   if (!mDcScreen)
   {
      fprintf(stderr, "Failed getting DC\n");
      return false;
   }
   if (!mDc)
      mDc = CreateCompatibleDC(mDcScreen);
   if (!mDc)
   {
      fprintf(stderr, "Failed creating DC\n");
      return false;
   }
   if (!mBmp)
      mBmp = CreateCompatibleBitmap(mDcScreen, mRect.right - mRect.left, mRect.bottom - mRect.top);
   if (!mBmp)
   {
      fprintf(stderr, "Failed creating bitmap\n");
      return false;
   }
   if (!SelectObject(mDc, mBmp))
   {
      fprintf(stderr, "Failed selecting bitmap\n");
      return false;
   }

   int result;
   if (!mBInfo.bmiHeader.biSizeImage)
   {
      result = GetDIBits(mDc, mBmp, 0, mBInfo.bmiHeader.biHeight, nullptr, &mBInfo, DIB_RGB_COLORS);
      if (!result)
      {
         fprintf(stderr, "Failed getting DIBits info\n");
         return false;
      }
      if (!mBInfo.bmiHeader.biSizeImage)
      {
         fprintf(stderr, "Got 0 image size!\n");
         return false;
      }
      mPixels = new Color[mBInfo.bmiHeader.biSizeImage];
   }

   result = GetDIBits(mDc, mBmp, 0, mBInfo.bmiHeader.biHeight, mPixels, &mBInfo, DIB_RGB_COLORS);
   if (!result)
   {
      fprintf(stderr, "Failed getting DIBits\n");
      return false;
   }

   return true;
}