// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIDTH 800
#define HEIGHT 600
#define SafeDelete(x) if (x) {delete x; x=NULL;}


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
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
