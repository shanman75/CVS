// AI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AI.h"
#include "D3DObject.h"
#include "Texture.h"
#include "ObjMgr.h"
#include "ObjEnemy.h"

#include "Timer.h"
#include <stdio.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hwnd;										// Handle to a Window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int ActiveApp=1;
CTexture *tex[4];
CObjMgr g_ObjMgr;
CTimer g_Time;
CTimer g_FireClock;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_AI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_AI);

	tex[0] = new CTexture("resource\\farsky.png",0xFFFF00FF);
	tex[1] = new CTexture("resource\\middleground.png",0xFFFF00FF);
	tex[2] = new CTexture("resource\\water.png",0xFFFF00FF);
	tex[3] = new CTexture("resource\\herodumdum2.bmp",0xFFFF00FF);
#define NUM_ENEMY1 10
	CObjEnemy *enemy1[NUM_ENEMY1];

	srand(5000);
	for (int x= 0; x<NUM_ENEMY1;x++) {
	enemy1[x] = new CObjEnemy;
	enemy1[x]->SetSpeed((rand()%10)-5,(rand()%10)-5);
	enemy1[x]->SetPosition(rand()%800,rand()%600);
	enemy1[x]->SetAccel((rand()%5),(rand()%5));
	enemy1[x]->Fire();
	g_ObjMgr.add(enemy1[x]);
	}


	// Main message loop:
	int m_cnt = 0;
	int zf = 4;
			
    while(TRUE)
       if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
        if(GetMessage(&msg,NULL,0,0)==0)return (int) msg.wParam; //get it
		TranslateMessage(&msg); 
		DispatchMessage(&msg); //translate it
       }
	   else if (ActiveApp) 
	   {
		    g_Time.UpdateClock();
			g_ObjMgr.move();
			if (g_FireClock.PeekTime() > 200) { enemy1[rand()%NUM_ENEMY1]->Fire(); g_FireClock.Reset();
												enemy1[rand()%NUM_ENEMY1]->SetAccel((rand()%15)-7,(rand()%15)-7);
			}

		    g_D3DObject->BeginPaint();
			g_D3DObject->Test(tex);
			g_ObjMgr.paint();
			g_D3DObject->EndPaint();
	   }
	   else WaitMessage();
	 //WaitMessage(); //process frame

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_AI);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	//(HBRUSH)(COLOR_BLACK);
	wcex.lpszMenuName	= NULL;
	//(LPCTSTR)IDC_AI;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_TOPMOST,szWindowClass, szTitle, WS_POPUP,
      0, 0, GetSystemMetrics(SM_CXSCREEN),
	  GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInstance,NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SetFocus(hWnd);
   g_D3DObject = new D3DObject;

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int wmId;
//	PAINTSTRUCT ps;
//	HDC hdc;

	switch (message) 
	{
    case WM_ACTIVATEAPP: ActiveApp=(int)wParam; break; //iconize
	case WM_DESTROY:
		delete g_D3DObject;
		delete tex[0];
		delete tex[1];
		delete tex[2];
		PostQuitMessage(0);
		break;
    case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0L;
}
/*
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
*/

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
