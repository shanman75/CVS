#pragma once

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Global Variables

extern HWND g_hWnd;        // Global HWND
extern HINSTANCE g_hInst; // Global hinstance
extern int g_ActiveApp;	   // If Application is Active


ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
void playlist();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
