#pragma once

#include "stdafx.h"

// Global Variables

extern HWND g_hWnd;        // Global HWND
extern HINSTANCE g_hInst; // Global hinstance
extern int g_ActiveApp;	   // If Application is Active



ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
