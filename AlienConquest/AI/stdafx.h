// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIDTH 800
#define HEIGHT 600
#define SafeDelete(x) if (x) {delete x; x=NULL;}
enum EN_PHASE{ INIT=0,GAME_PHASE, TITLE_PHASE, CREDIT_PHASE, MENU_PHASE, LOADING_PHASE, QUIT ,CREDITS_PHASE, OPTIONS_PHASE};
enum EN_SUBPHASE { NONE, GAME_OVER, WINNER, LEVELUP };

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

extern HWND hwnd;
extern HINSTANCE hInst;

// TODO: reference additional headers your program requires here
