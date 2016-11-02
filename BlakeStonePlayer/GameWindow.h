#pragma once

#include <string>

#include <Windows.h>

union Color
{
   uint32_t u4;
   struct
   {
      uint8_t a, b, g, r;
   };
   uint8_t u[4];
};

class GameWindow
{
   HWND mWnd;
   std::string mTitle;

   RECT mRect;

   HDC mDcScreen;
   HDC mDc;
   HBITMAP mBmp;
   BITMAPINFO mBInfo;

   Color *mPixels;

public:
   GameWindow() : mWnd(nullptr), mRect(), mDcScreen(nullptr), mDc(nullptr), mBmp(nullptr), mBInfo(), mPixels(nullptr)
   {
      mBInfo.bmiHeader.biSize = sizeof(mBInfo.bmiHeader);
      mBInfo.bmiHeader.biPlanes = 1;
      mBInfo.bmiHeader.biBitCount = 32;
      mBInfo.bmiHeader.biCompression = BI_RGB;
   }
   bool open(const char *name);
   bool screenshot();
   ~GameWindow()
   {
      delete[] mPixels;
      if (mBmp)
         DeleteObject(mBmp);
      if (mDc)
         DeleteDC(mDc);
      if (mDcScreen)
         ReleaseDC(nullptr, mDcScreen);
   }

   Color *operator[](unsigned n) const
   {
      return mPixels + n * mBInfo.bmiHeader.biWidth;
   }
};

