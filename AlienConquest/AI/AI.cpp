// AI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AI.h"
#include "D3DObject.h"
#include "D3DInput.h"
#include "Texture.h"
#include "ObjMgr.h"
#include "dmusic.h"

#include "Timer.h"
#include <stdio.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hwnd;										// Handle to a Window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int ActiveApp=1;
CTimer g_Time;
CTimer g_FireClock;
CMidiMusic *g_Midi;

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

	/*
#define NUM_ENEMY1 1
#define NUM_ENEMY2 1
#define NUM_ENEMY3 1

	CObjEnemy  *enemy1[NUM_ENEMY1];
	CObjEnemy2 *enemy2[NUM_ENEMY2];
	CObjEnemy3 *enemy3[NUM_ENEMY3];

	srand(GetTickCount());
	for (int x= 0; x<NUM_ENEMY1;x++) {
	enemy1[x] = new CObjEnemy;
	enemy1[x]->SetSpeed(3,4);
	enemy1[x]->SetPosition((float)(rand()%800),(float)(rand()%600));
//	enemy1[x]->SetAccel((float)(rand()%5),(float)(rand()%5));
	g_ObjMgr->add(enemy1[x]);
	}
	for (int x= 0; x<NUM_ENEMY2;x++) {
		enemy2[x] = new CObjEnemy2;
		enemy2[x]->SetSpeed((float)((rand()%10)-5),(float)((rand()%10)-5));
		enemy2[x]->SetPosition((float)(rand()%800),(float)(rand()%600));
//		enemy2[x]->SetAccel((float)(rand()%5),(float)(rand()%5));
		g_ObjMgr->add(enemy2[x]);
	}
	for (int x= 0; x<NUM_ENEMY3;x++) {
		enemy3[x] = new CObjEnemy3;
		enemy3[x]->SetSpeed((float)((rand()%10)-5),(float)((rand()%10)-5));
		enemy3[x]->SetPosition((float)(rand()%800),(float)(rand()%600));
//		enemy3[x]->SetAccel((float)(rand()%5),(float)(rand()%5));
		g_ObjMgr->add(enemy3[x]);
	}
*/
	CBkGround *bkg=new CBkGround;
	g_ObjMgr->add(bkg);
	CHero *hero;
	hero = new CHero;
	hero->SetPosition(50,50);
	g_ObjMgr->add(hero);


	// Main message loop:
	int m_cnt = 0;
	int zf = 4;

	g_Midi = new CMidiMusic;
	g_Midi->Initialize(TRUE);
	DWORD dwcount;
	INFOPORT Info; // INFOPORT structure to store port information 
	BOOL bSelected;

	dwcount=0;
	bSelected=FALSE;
	
	 // Port enumeration  phase 
	 // It is necessary to supply a port counter 
	while (g_Midi->PortEnumeration(dwcount,&Info)==S_OK)
	{
		// Ensure it is an output hardware device
		if (Info.dwClass==DMUS_PC_OUTPUTCLASS) 
		{
			if (!((Info.dwFlags & DMUS_PC_SOFTWARESYNTH) || bSelected))
			{
				// Select the enumerated port 
				g_Midi->SelectPort(&Info);
				bSelected=TRUE;
			}
		}
	
	dwcount++;  // It is necessary
	}

	 // Read the file and specify if it is a mid file or not 
	g_Midi->LoadMidiFromFile("resource/Doctor Who 5.mid",FALSE);
//	g_Midi->LoadMidiFromFile("c:/windows/media/town.mid",FALSE);
	 // Play the file
	g_Midi->Play();
	g_Midi->Stop();
	g_Midi->SetRepeat(TRUE);
	g_Midi->Play();

	while(TRUE)
       if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
        if(GetMessage(&msg,NULL,0,0)==0)return (int) msg.wParam; //get it
		TranslateMessage(&msg); 
		DispatchMessage(&msg); //translate it
       }
	   else if (ActiveApp) 
	   {
			g_D3DInput->GetInput(hero);			
		    g_Time.UpdateClock();
			g_ObjMgr->spawn();
			g_ObjMgr->coldet();
			g_ObjMgr->move();
			// Collision detection
		    g_D3DObject->BeginPaint();
			g_ObjMgr->paint();
			g_D3DObject->PaintText();
			g_D3DObject->EndPaint();
	   }
	   else WaitMessage();
	 //WaitMessage(); //process frame

	 // Stop it
	g_Midi->Stop();
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
   g_D3DInput = new D3DInput;
   g_ObjMgr = new CObjMgr;

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
		OutputDebugString("Deleting global Object Manager\n");
		delete g_ObjMgr;
		OutputDebugString("Deleting global D3D Input\n");
		delete g_D3DInput;
		OutputDebugString("Deleting global D3D Object\n");
		delete g_D3DObject;
		OutputDebugString("Deleting global Midi\n");
		delete g_Midi;
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
