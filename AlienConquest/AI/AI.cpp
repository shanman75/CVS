// AI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AI.h"
#include "D3DObject.h"
#include "D3DInput.h"
#include "Texture.h"
#include "objmgr.h"
#include "dmusic.h"
#include "text.h"

#include "Timer.h"
#include <stdio.h>

enum EN_PHASE{ GAME_PHASE=1, TITLE_PHASE, CREDIT_PHASE, MENU_PHASE, LOADING_PHASE };

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hwnd;										// Handle to a Window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int ActiveApp=1;
CTimer g_Time;
CTimer g_FireClock;
CTimer g_PhaseTimer;
CMidiMusic *g_Midi;
CObj *g_preload[255];							// Array of preloaded objects
CHero *g_hero;
CObjBmp *g_ScrBmp;
int g_preload_num;								// Number of preloads
EN_PHASE g_phase=GAME_PHASE;

void Init_Level(int levelnum);
void Init_Midi(void);
void Cleanup_Level(int levelnum);
void Display_Bitmap(char *bmpname, int numsec);
void Process_Phase(void);

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
//	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_AI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

//	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_AI);


	Init_Midi();
	g_phase=CREDIT_PHASE;
	g_ScrBmp = new CObjBmp("resource/credits.bmp");
    g_Time.UpdateClock();
	g_PhaseTimer.Reset();
	g_Midi->LoadMidiFromFile("resource/peanuts.mid",FALSE);
	g_Midi->Play();

	while(TRUE)
       if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
        if(GetMessage(&msg,NULL,0,0)==0)return (int) msg.wParam; //get it
		TranslateMessage(&msg); 
		DispatchMessage(&msg); //translate it
       }
	   else if (ActiveApp) 
	   {
		   Process_Phase();
	   }
	   else {
		   g_Midi->Pause();
		   g_Time.Pause();
		   WaitMessage();
		   g_Time.TogglePause();
		   g_Midi->Resume();
	   }

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
   if (g_D3DObject == NULL) return FALSE;
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


void Init_Level (int levelnum)
{
	// Preload graphics .. there is only one level....

	Cleanup_Level(1);
	switch (levelnum) {
		case 1:
			g_preload[g_preload_num++] = new CHero;
			g_preload[g_preload_num++] = new CObjEnemy;
			g_preload[g_preload_num++] = new CObjEnemyWeapon;				   
			g_preload[g_preload_num++] = new CObjEnemy2;
			g_preload[g_preload_num++] = new CObjEnemyWeapon2;
			g_preload[g_preload_num++] = new CObjEnemy3;
			g_preload[g_preload_num++] = new CObjEnemyWeapon3;
			g_preload[g_preload_num++] = new CObjHeroWeaponMain;
			g_preload[g_preload_num++] = new CObjHeroWeaponMissile;
			g_preload[g_preload_num++] = new CBkGround;

			break;
		default:
			break;
	}
}

void Start_Level(int levelnum)
{
	// Actually start the level...
	switch (levelnum) {
		case 1:
			// Midi for level 1
			delete g_ObjMgr; g_ObjMgr = new CObjMgr;
//			g_ObjMgr->reset();
			g_Midi->LoadMidiFromFile("resource/Doctor Who 5.mid",FALSE);
		//	g_Midi->LoadMidiFromFile("c:/windows/media/town.mid",FALSE);
			// Play the file
			g_Midi->Play();
			g_Midi->Stop();
			g_Midi->SetRepeat(TRUE);
			g_Midi->Play();

			g_ObjMgr->add(new CBkGround);
			g_hero = new CHero;
			g_hero->SetPosition(150,150);
			g_ObjMgr->add(g_hero);
			break;
		default:
			break;
	}
}

void Cleanup_Level(int levelnum)
{
   while (g_preload_num > 0)
   {
	   delete g_preload[--g_preload_num];
	   g_preload[g_preload_num] = NULL;
   }
}

void Display_Bitmap(char *bmpname, int numsec)
{

}

void Init_Midi(void)
{
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
}

void Process_Phase(void)
{
    g_Time.UpdateClock();
	switch (g_phase)
	{
	case CREDIT_PHASE:
			g_D3DInput->GetInput(g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 6000 || g_ScrBmp->done())
			{
				delete g_ScrBmp;
				g_ScrBmp = new CObjBmp("resource/title.bmp");
				g_phase=TITLE_PHASE;
				g_PhaseTimer.Reset();
			}
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case TITLE_PHASE:
			g_D3DInput->GetInput(g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 6000 || g_ScrBmp->done())
			{
				delete g_ScrBmp;
				g_ScrBmp = new CObjBmp("resource/menu.bmp");
				g_phase=MENU_PHASE;
				g_PhaseTimer.Reset();
			} 
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case MENU_PHASE:
			g_D3DInput->GetInput(g_ScrBmp);					 
			if (g_ScrBmp->done())
			{
				delete g_ScrBmp;
				g_phase=GAME_PHASE;
				Init_Level(1);
				Start_Level(1);
				g_PhaseTimer.Reset();
			} 
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case GAME_PHASE:
			g_D3DInput->GetInput(g_hero);			
			g_ObjMgr->spawn();
			g_ObjMgr->coldet();
			g_ObjMgr->move();
			// Collision detection
		    g_D3DObject->BeginPaint();
			g_ObjMgr->paint();
			g_D3DObject->PaintText();
			g_D3DObject->EndPaint();
			if (g_hero->GetLives() == 0) {
				Cleanup_Level(1);
				g_phase = MENU_PHASE;
				delete g_ObjMgr;
				g_ObjMgr = new CObjMgr;
				g_PhaseTimer.Reset();
				g_ScrBmp = new CObjBmp("resource/menu.bmp");
			}
			break;
	default:
		break;
	}

}