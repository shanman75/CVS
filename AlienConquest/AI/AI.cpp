// AI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AI.h"
#include "D3DObject.h"
#include <stdio.h>


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hwnd;										// Handle to a Window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int ActiveApp=1;

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

	IDirect3DSurface8* background[3]; //surface for image
	IDirect3DSurface8* main_frame;    // main_frame
	IDirect3DTexture8* texture[3];	// textures

	D3DSURFACE_DESC background_desc[3]; // descriptions for surfaces
	D3DSURFACE_DESC main_desc; // descriptions for main_frame
	D3DXIMAGE_INFO text_desc[3];

	int background_cnt[3];
				background_cnt[0] = 0;
			background_cnt[1] = 0;
			background_cnt[2] = 0;


    //get surface for frame 0, this line of code is from InitD3D8 in Setup.cpp of Demo 1

	char *filename = "bg.bmp";

	if (g_D3DObject->LoadSurfaceFromFile("resource\\farsky.png",&background[0]) != D3D_OK) return FALSE;
	if (g_D3DObject->LoadSurfaceFromFile("resource\\middleground.png",&background[1]) != D3D_OK) return FALSE;
	if (g_D3DObject->LoadSurfaceFromFile("resource\\water.png",&background[2]) != D3D_OK) return FALSE;
	if (g_D3DObject->MakeScreenSurface(0,0,(D3DFORMAT)D3DFMT_UNKNOWN,&main_frame) != D3D_OK) return FALSE;

	if (g_D3DObject->LoadTextureFromFile("resource\\farsky.png",&texture[0],0xFFFF00FF,&text_desc[0]) != D3D_OK) return FALSE;
	if (g_D3DObject->LoadTextureFromFile("resource\\middleground.png",&texture[1],0xFFFF00FF,&text_desc[1]) != D3D_OK) return FALSE;
	if (g_D3DObject->LoadTextureFromFile("resource\\water.png",&texture[2],0xFFFF00FF,&text_desc[2]) != D3D_OK) return FALSE;

	//g_D3DObject->CopySurfaceToTexture(background[1],texture[1]);
	g_D3DObject->CopySurfaceToTexture(background[2],texture[2]);

	for (int x = 0; x < 3; x++) 
		background[x]->GetDesc(&background_desc[x]);
	main_frame->GetDesc(&main_desc);
  
	// Main message loop:
	int m_cnt = 0;
    while(TRUE)
       if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
        if(GetMessage(&msg,NULL,0,0)==0)return (int) msg.wParam; //get it
		TranslateMessage(&msg); 
		DispatchMessage(&msg); //translate it
//		if (ActiveApp) g_D3DObject->PaintFrame(background);
       }
	   else if (ActiveApp) 
	   {
		   	RECT rc;
			POINT pnt;
			IDirect3DSurface8* cur_surf;
			D3DSURFACE_DESC cur_desc;
			
			int xx = 0;
			int cur_cnt;
			pnt.x = 0;

			for (xx=0; xx <= 2; xx++) {
				cur_surf = background[xx];
				cur_desc = background_desc[xx];
				cur_cnt = background_cnt[xx];
				background_cnt[xx] = (int)(background_cnt[xx]+main_desc.Width > background_desc[xx].Width ? 0 : background_cnt[xx]+ xx*1.5);

				pnt.y = 0;
				if (xx == 1) pnt.y = main_desc.Height-background_desc[2].Height-cur_desc.Height;
				if (xx == 2) pnt.y = main_desc.Height-cur_desc.Height;
				SetRect(&rc,0+cur_cnt,0,(main_desc.Width+cur_cnt) % cur_desc.Width,cur_desc.Height);
	//			g_D3DObject->CopyRects(cur_surf,&rc,1,main_frame,&pnt);
				//g_D3DObject->CopyBackTexture(cur_surf,&rc,1,&pnt,xx);
			}
	//		g_D3DObject->PaintFrame(main_frame);
//			for (int x = 1; x < 100; x++)
			g_D3DObject->Test(texture,text_desc);
			//g_D3DObject->DrawTextStr(500,500,0xFFFF00FF,"Hellloooo nurse");
	   }
	   else WaitMessage();
	 //WaitMessage(); //process frame

	   for (int y = 0; y <= 2 ; y++)
		   background[y]->Release();

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
