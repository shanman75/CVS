/////////////////////////////////////////////////////////////////////////////
//
// 3D Math Primer for Games and Graphics Development
//
// WinMain.h - Windows common stuff
//
// Visit gamemath.com for the latest version of this file.
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// Main application window handle

extern HWND	gHwndApp;

// Main application instance handle

extern HINSTANCE	gHInstApp;

// Should our program idle when we are in the background?

extern bool	gIdleInBackground;

// Create/destroy the main application window

void	createAppWindow(const char *title);
void	destroyAppWindow();

// Perform per-frame tasks.  This is called from toggle()

void	idle();
