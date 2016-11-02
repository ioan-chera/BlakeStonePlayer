// BlakeStonePlayer.cpp : Defines the entry point for the console application.
//

#include <Windows.h>
#include "stdafx.h"
#include "GameWindow.h"

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
   GameWindow window;
   if (!window.open(gWindowName))
   {
      return EXIT_FAILURE;
   }

   hitEnter();
   Sleep(1000);
   hitEnter();
   Sleep(1000);
   hitEnter();
   Sleep(1000);
   hitEnter();
   Sleep(5000);
   hitEnter();

    return EXIT_SUCCESS;
}

