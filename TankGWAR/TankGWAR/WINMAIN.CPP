/////////////////////////////////////////////////////////////////////////////
//
// 3D Math Primer for Games and Graphics Development
//
// WinMain.cpp - Windows common stuff
//
// Visit gamemath.com for the latest version of this file.
//
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WinMain.h"
#include <assert.h>

//#include "Renderer.h"
//#include "Input.h"

/////////////////////////////////////////////////////////////////////////////
//
// global variables
//
/////////////////////////////////////////////////////////////////////////////

HWND		gHwndApp = NULL;
HINSTANCE	gHInstApp;
bool		gQuitFlag = false;
bool		gIdleInBackground = true;

extern char g_errMsg[]; //error message if we had to abort
extern bool g_errorExit; // true if we had to abort

/////////////////////////////////////////////////////////////////////////////
//
// local stuff
//
/////////////////////////////////////////////////////////////////////////////

// Is our app in the foreground?

static bool appInForeground = true;

// Change the current directory to the one containing the executable.
// This simplifies a lot of stuff

static void	switchToDirectory() {

	// Get the directory of the command line filename and module

	char	programDir[256];
	GetModuleFileName(gHInstApp,programDir,256);

	// Strip off the program name from programDir

	int i = (int) strlen(programDir);
	while (i > 0) {
		if (programDir[i] == '\\') {
			programDir[i] = 0;
			break;
		}
		i--;
	}

	// Switch to the program directory

	SetCurrentDirectory(programDir);
}

/*
inline void	updateMousePos(LPARAM lParam) {
	gMouse.cursorMoved(LOWORD(lParam), HIWORD(lParam));
}
*/

/////////////////////////////////////////////////////////////////////////////
//
// Externals
//
/////////////////////////////////////////////////////////////////////////////

// mainProgram() - this is the function that your app defines

extern void	mainProgram();

/////////////////////////////////////////////////////////////////////////////
//
// Global code
//
/////////////////////////////////////////////////////////////////////////////

/*

	WindowProc - The windows message routine

*/

long FAR PASCAL WindowProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {

	// Make sure this message is for our app

	if (hWnd != gHwndApp) {
		return (long) DefWindowProc(hWnd,message,wParam,lParam);
	}

	// Process the message

	switch(message)	{
		case WM_ACTIVATEAPP :
			appInForeground = wParam;
			if (appInForeground) {
//				gRenderer.videoRestore();
			} else {
//				gRenderer.videoSave();
			}
			break;

		case WM_CLOSE:
		case WM_DESTROY:
			// Display error message dialog box, if there is one
			if(g_errorExit)
				MessageBox(NULL, g_errMsg, "FATAL ERROR", MB_OK | MB_ICONERROR);
			// Now gettaoutahere
			gQuitFlag = true;
			break;

		case WM_KEYDOWN :
		case WM_SYSKEYDOWN :
		{
			// Extract keyboard scan code

			int keyCode = (int) (lParam >> 16) & 511;

			// Press the key

/*
			gKeyboard.press(keyCode);

			// Queue an event

			KeyEvent event;
			event.type = eKeyEventPressed;
			event.keyCode = keyCode;
			event.modifiers = gKeyboard.getModifiers();
			gKeyboard.queueEvent(event);

			// Disable default windows message processing for
			// a few keys.  We don't want F10 to activate the
			// system menu, or alt-F4 to terminate the app.

			switch (keyCode) {
				case kKeyF4:
				case kKeyF10:
				case kKeyLeftAlt:
				case kKeyRightAlt:
					return 0;
			}
*/

		} break;

		case WM_CHAR :
		{

			// Queue an event

/*
            KeyEvent event;
			event.type = eKeyEventChar;
			event.asciiCode = (char)wParam;
			if (event.asciiCode != 0) {
				event.modifiers = gKeyboard.getModifiers();
				gKeyboard.queueEvent(event);
			}
		} break;
*/

		case WM_KEYUP :
		case WM_SYSKEYUP :
		{

			// Extract keyboard scan code

			int keyCode = (lParam >> 16) & 511;

			// Release the key

			gKeyboard.debounce(keyCode);

			// Queue an event

			KeyEvent event;
			event.type = eKeyEventReleased;
			event.keyCode = keyCode;
			event.modifiers = gKeyboard.getModifiers();
			gKeyboard.queueEvent(event);
		} break;

		case WM_SETCURSOR:
			if (!gMouse.isOSCursorVisible()) {
				::SetCursor(NULL);
				return 1;
			}
			break;

		case WM_MOUSEMOVE :
			updateMousePos(lParam);
			break;

		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN :
			updateMousePos(lParam);
			gMouse.press(Mouse::eButtonLeft);
			break;

		case WM_LBUTTONUP :
			updateMousePos(lParam);
			gMouse.debounce(Mouse::eButtonLeft);
			break;

		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN :
			updateMousePos(lParam);
			gMouse.press(Mouse::eButtonRight);
			break;

		case WM_RBUTTONUP :
			updateMousePos(lParam);
			gMouse.debounce(Mouse::eButtonRight);
			break;

		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN :
			updateMousePos(lParam);
			gMouse.press(Mouse::eButtonMiddle);
			break;

		case WM_MBUTTONUP :
			updateMousePos(lParam);
			gMouse.debounce(Mouse::eButtonMiddle);
			break;

		case 523: // WM_XBUTTONDOWN
			updateMousePos(lParam);
			switch (HIWORD(wParam)) {
				case 1:
					gMouse.press(Mouse::eButtonX1);
					break;
				case 2:
					gMouse.press(Mouse::eButtonX2);
					break;
				default:
					assert(false); // hey - they invented a new button!
			}
			break;

		case 524: // WM_XBUTTONUP
			updateMousePos(lParam);
			switch (HIWORD(wParam)) {
				case 1:
					gMouse.debounce(Mouse::eButtonX1);
					break;
				case 2:
					gMouse.debounce(Mouse::eButtonX2);
					break;
				default:
					assert(false); // hey - they invented a new button!
			}
			break;

		case 522: // WM_MOUSEWHEEL
		{
			updateMousePos(lParam);
			int delta = (signed short)HIWORD(wParam);
			gMouse.wheelMoved((float)delta / 120.0f);
		} break;
	}

	// Return the default window procedure

	return DefWindowProc(hWnd,message,wParam,lParam);

}

/*
	idle - Perform frame-time processing tasks
*/

void	idle() {

	// Check if there is a message for us, repeat if program is not
	// in the foreground

	for (;;) {

		// Process all pending Windows messages

		MSG	msg;
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
			TranslateMessage(&msg); 
			DispatchMessage(&msg);
		}

		// If we're in the foreground, return to normal message processing

		if (appInForeground) break;

		// If we've been told to quit the app, then bail this loop

		if (gQuitFlag) break;

		// We're in the background - always give back some time

		Sleep(20);

		// Check if we're supposed to idle in the background

		if (!gIdleInBackground) break;

		// Repeat until program is in the foreground,
		// or we signal program termination
	}

}


/*

	createAppWindow - Create a new window for the application

*/

void	createAppWindow(const char *title) {

	// Register the window class

	WNDCLASS	wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = gHInstApp;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;
	RegisterClass(&wc);

	// Create our window

	gHwndApp = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME,
		title,
		title,
		WS_POPUP | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW,
		0,
		0,
		0,
		0,
		NULL,
		NULL,
		gHInstApp,
		NULL
	);
}


/*

	destroyAppWindow - Destroy the application window

*/

void	destroyAppWindow() {

	// Make sure we exist

	if (gHwndApp != NULL) {

		// Destroy it

		DestroyWindow(gHwndApp);
		gHwndApp = NULL;
	}
}

/*

	WinMain - Windows entry point

*/

int PASCAL WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow) {

	// Save off our instance

	gHInstApp = hInstance;

	// Switch directory to the one containing the executable

	switchToDirectory();

	// Run the app initialization

	mainProgram();

	// Done

	return 0;
}
