#include "StdAfx.h"
#include "mp3aud.h"
#include "mainwnd.h"
#include "D3DObject.h"
#include "D3DInput.h"
#include "ObjMgr.h"
#include "resource.h"
#include "tankgwar.h"
#include <stdio.h>

// Globals
#define MAX_LOADSTRING 100

HWND g_hWnd;
HINSTANCE g_hInst;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int g_ActiveApp=1;
cmp3stream *g_mp3_1;							//first mp3 sound


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
	//wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TANKGWAR);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
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

   g_hInst = hInstance; // Store instance handle in our global variable

   DWORD style;

   if (WINDOWED) 
	   style = WS_POPUP | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW;
   else
	   style = WS_POPUP;

   hWnd = CreateWindowEx(
		WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME,  //	   WS_EX_TOPMOST,
	   szWindowClass, 
	   szTitle, 
	   style,
       0, 
	   0, 
	   0, //GetSystemMetrics(SM_CXSCREEN),
	   0, //GetSystemMetrics(SM_CYSCREEN),
	   NULL,
	   NULL,
	   hInstance,
	   NULL);

   if (!hWnd)
      return FALSE;

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SetFocus(hWnd);
   
   /*       D3D Stuff               */

   OutputDebugString("Creating D3D Object\n");
   g_D3DObject = new D3DObject;
   if (g_D3DObject == NULL) return FALSE;
   OutputDebugString("Creating D3D Input\n");
   g_D3DInput = new D3DInput;
   OutputDebugString("Creating Obj Mgr\n");
   g_ObjMgr = new CObjMgr;
/*	
   OutputDebugString("Creating Sound Manager\n");
   g_SndMgr = new CSoundManager(hwnd);
   */
   OutputDebugString("Done creating global objects\n\n");
   /*       D3D Stuff End             */

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
char debg[255];
	switch (message) 
	{
    case WM_ACTIVATEAPP: g_ActiveApp=(int)wParam; sprintf (debg,"Activate app called %i\n",(int)wParam); OutputDebugString(debg);
                         g_time.UnPause();
                          break; //iconize
	  case WM_DESTROY:
		/*
		OutputDebugString("Deleting global Midi\n");
		SafeDelete(g_Midi);
		OutputDebugString ("Deleting Sound Manager\n");
		SafeDelete(g_SndMgr);
		*/
		OutputDebugString("Deleting global Object Manager\n");
		SafeDelete(g_ObjMgr);
		OutputDebugString("Deleting global D3D Input\n");
		SafeDelete(g_D3DInput);
    OutputDebugString("Deleting MP3 object\n");
    SafeDelete(g_mp3_1);
    CoUninitialize();
   	g_MainDestroy();
		
		OutputDebugString("Deleting global D3D Object\n");
		SafeDelete(g_D3DObject);
		PostQuitMessage(0);
		break;
    case WM_KEYDOWN:
		  if (wParam == VK_ESCAPE) DestroyWindow(g_hWnd);  // Remark this out to remove the ESCAPE
		                                                   // Key exiting functionality
    break;
    case WM_USER:      g_mp3_1->OnGraphEvent();         // handles events
      break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0L;
}


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
	LoadString(hInstance, IDC_TANKGWAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
  CoInitialize(NULL);
  g_mp3_1=new cmp3stream(g_hWnd);
  g_mp3_1->CreateGraph(NULL);


	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TANKGWAR);

	g_MainInit();

	while(TRUE)
	{
       if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
        if(GetMessage(&msg,NULL,0,0)==0)return (int) msg.wParam; //get it
		      TranslateMessage(&msg); 
		      DispatchMessage(&msg); //translate it
       }
       else if (g_ActiveApp) {
         g_mp3_1->startsound();
         g_MainGameLoop();
	     }
	   else {
		   g_mp3_1->stopsound();
		   g_time.Pause();
		   WaitMessage();
	   }
   }

	return (int) msg.wParam;
}
