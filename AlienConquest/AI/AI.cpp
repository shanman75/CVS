// AI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AI.h"
#include "D3DObject.h"
#include "D3DInput.h"
#include "Texture.h"
#include "objmgr.h"
#include "dmusic.h"
#include "Objmenu.h"
#include "Sound.h"

#include "Timer.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst=0;								// current instance
HWND hwnd=0;										// Handle to a Window
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
int ActiveApp=1;
CTimer g_Time;
CTimer g_FireClock;
CTimer g_PhaseTimer;
CMidiMusic *g_Midi = NULL;
CObj *g_preload[255];							// Array of preloaded objects
CHero *g_hero = NULL;
CHero2 *g_hero2 = NULL;
CObj *g_ScrBmp = NULL;
CSoundManager *g_SndMgr = NULL;
int g_numenemies=0;
int g_levelnum=1;
BOOL g_grINIT = FALSE;

int g_preload_num=0;								// Number of preloads
EN_PHASE g_phase=INIT;
EN_SUBPHASE g_subphase=NONE;


void Init_Level(int levelnum);
void Init_Midi(void);
void Cleanup_Level(int levelnum);
void Process_Phase(void);
void Start_Level(int levelnum);
void Switch_Phase(EN_PHASE new_phase);

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

	OutputDebugString("INIT MIDI\n");
	Init_Midi();

	OutputDebugString("Switch to Credit Phase\n");
    Switch_Phase(CREDIT_PHASE);

	while(TRUE)
       if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){ //if message waiting
        if(GetMessage(&msg,NULL,0,0)==0)return (int) msg.wParam; //get it
		TranslateMessage(&msg); 
		DispatchMessage(&msg); //translate it
       }
	   else if (ActiveApp) 
	   {
		   Process_Phase();
		   if (g_phase == QUIT)
		   {
				g_Midi->Stop();
				DestroyWindow(hwnd);
				// Cleanup
		   }
	   }
	   else {
		   g_Midi->Pause();
		   if (g_phase == GAME_PHASE)
		   g_Time.Pause();
		   WaitMessage();
//		   g_Time.TogglePause();
		   g_Midi->Resume();
	   }

	// Stop it


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

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowEx(WS_EX_TOPMOST,szWindowClass, szTitle, WS_POPUP,
      0, 0, GetSystemMetrics(SM_CXSCREEN),
	  GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInstance,NULL);

   if (!hWnd)
      return FALSE;

   hwnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   SetFocus(hWnd);
   OutputDebugString("Creating D3D Object\n");
   g_D3DObject = new D3DObject;
   if (g_D3DObject == NULL) return FALSE;
   OutputDebugString("Creating D3D Input\n");
   g_D3DInput = new D3DInput;
   OutputDebugString("Creating Obj Mgr\n");
   g_ObjMgr = new CObjMgr;
   OutputDebugString("Creating Sound Manager\n");
   g_SndMgr = new CSoundManager(hwnd);
   OutputDebugString("Done creating global objects\n\n");

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
		Cleanup_Level(0);
		OutputDebugString("Deleting global Midi\n");
		SafeDelete(g_Midi);
		OutputDebugString ("Deleting Sound Manager\n");
		SafeDelete(g_SndMgr);
		OutputDebugString("Deleting global Object Manager\n");
		SafeDelete(g_ObjMgr);
		OutputDebugString("Deleting global D3D Input\n");
		SafeDelete(g_D3DInput);
		OutputDebugString("Deleting global D3D Object\n");
		SafeDelete(g_D3DObject);
		PostQuitMessage(0);
		break;
    case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
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
		break;
	case WM_COMMAND:
		break;
	}
	return FALSE;
}


void Init_Level (int levelnum)
{
	// Preload graphics .. there is only one level....

	switch (levelnum) {
		case 0:
			g_preload[g_preload_num++] = new CHero;
			g_preload[g_preload_num++] = new CHero2;
//			g_preload[g_preload_num++] = new CObjEnemy;
//			g_preload[g_preload_num++] = new CObjEnemyWeapon;				   
			g_preload[g_preload_num++] = new CObjEnemy2;
			g_preload[g_preload_num++] = new CObjEnemyWeapon2;
			g_preload[g_preload_num++] = new CObjEnemy3;
			g_preload[g_preload_num++] = new CObjEnemyWeapon3;
			g_preload[g_preload_num++] = new CObjHeroWeaponMain;
			g_preload[g_preload_num++] = new CObjHeroWeaponMissile;
			g_preload[g_preload_num++] = new CObjHeroWeaponLaser;
			//g_preload[g_preload_num++] = new CBkGround;
			g_preload[g_preload_num++] = new CObjPowerUp;

			g_SndMgr->load("resource/menu/menumove.wav",1);
			g_SndMgr->load("resource/menu/menuselect.wav",1);

			g_SndMgr->load("resource/enemies/enemy2/fire.wav",2);
			g_SndMgr->load("resource/enemies/enemy2/jet.wav",2);
			g_SndMgr->load("resource/enemies/enemy2/hit.wav",2);
			g_SndMgr->load("resource/enemies/enemy2/dead.wav",2);

			g_SndMgr->load("resource/enemies/enemy3/fire.wav",2);
			g_SndMgr->load("resource/enemies/enemy3/jet.wav",2);
			g_SndMgr->load("resource/enemies/enemy3/hit.wav",2);
			g_SndMgr->load("resource/enemies/enemy3/dead.wav",2);

			g_SndMgr->load("resource/hero/fire.wav",1);
			g_SndMgr->load("resource/hero/missile.wav",2);
			g_SndMgr->load("resource/hero/laser.wav",1);
			g_SndMgr->load("resource/hero/hit.wav",1);
			g_SndMgr->load("resource/hero/dead.wav",1);

			g_SndMgr->load("resource/sounds/gameover.wav",1);
			g_SndMgr->load("resource/sounds/pause.wav",1);
			g_SndMgr->load("resource/sounds/unpause.wav",1);
			g_SndMgr->load("resource/sounds/win.wav",1);
			g_SndMgr->load("resource/sounds/levelup.wav",1);
			break;
		case 1:
			g_numenemies=10;
			break;

		case 2:
			g_numenemies=40;
			break;
		case 3:
			g_numenemies=80;
			break;
		default:
			break;
	}
}

void Start_Level(int levelnum)
{
	// Actually start the level...
	switch (levelnum) {
		case 0:
			break;
		case 1:
			//g_ObjMgr->reset();
			// Midi for level 1
			g_subphase = NONE;
		//	g_Midi->LoadMidiFromFile("c:/windows/media/town.mid",FALSE);
			// Play the file

			g_Midi->Stop();
			g_Midi->LoadMidiFromFile("resource/sounds/level1.mid",FALSE);
			g_Midi->Play();
			g_Midi->Stop();
			g_Midi->SetRepeat(TRUE);
			g_Midi->Play();

			g_ObjMgr->add(new CBkGround);

			// Player 1
			g_hero = new CHero;
			g_hero->SetPosition(35,150);
			g_hero->InitCharacter();
			g_hero->Live();
			g_ObjMgr->add(g_hero);

			// Player 2 
			g_hero2 = new CHero2;
			g_hero2->SetPosition(35,350);
			g_hero2->InitCharacter();
			g_ObjMgr->add(g_hero2);
			break;

		case 2:
			//g_ObjMgr->reset();
			// Midi for level 2
			g_subphase = NONE;
		//	g_Midi->LoadMidiFromFile("c:/windows/media/town.mid",FALSE);
			// Play the file
			g_Midi->Stop();
			g_Midi->LoadMidiFromFile("resource/sounds/level2.mid",FALSE);
			g_Midi->Play();
			g_Midi->Stop();
			g_Midi->SetRepeat(TRUE);
			g_Midi->Play();

			g_ObjMgr->add(new CBkGround);

			// Player 1
			g_hero = new CHero;
			g_hero->SetPosition(35,150);
			g_ObjMgr->add(g_hero);

			// Player 2 
			g_hero2 = new CHero2;
			g_hero2->SetPosition(35,350);
			g_ObjMgr->add(g_hero2);
			
			break;
		case 3:
			//g_ObjMgr->reset();
			// Midi for level 2
			g_subphase = NONE;
		//	g_Midi->LoadMidiFromFile("c:/windows/media/town.mid",FALSE);
			// Play the file

			g_Midi->Stop();
			g_Midi->LoadMidiFromFile("resource/sounds/level3.mid",FALSE);
			g_Midi->Play();
			g_Midi->Stop();
			g_Midi->SetRepeat(TRUE);
			g_Midi->Play();

			g_ObjMgr->add(new CBkGround);

			// Player 1
			g_hero = new CHero;
			g_hero->SetPosition(35,150);
			g_ObjMgr->add(g_hero);

			// Player 2 
			g_hero2 = new CHero2;
			g_hero2->SetPosition(35,350);
			g_ObjMgr->add(g_hero2);
			
			break;

		default:
			break;
	}
	g_Time.UnPause();
}

void Cleanup_Level(int levelnum)
{
	if (levelnum == 0)
	while (g_preload_num > 0)
	{
	   delete g_preload[--g_preload_num];
	   g_preload[g_preload_num] = NULL;
	}
	g_hero = NULL;
	g_hero2 = NULL;
//	   g_SndMgr->clear();
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

void Switch_Phase(EN_PHASE new_phase)
{
	char buff[500];
	sprintf(buff,"Switching from %i phase to %i\n",(int)g_phase,(int)new_phase);
	OutputDebugString(buff);

	if (g_phase == new_phase) return;
	// HELP ME, I'VE FALLEN AND I CAN'T GET UP!
	// Tear down the previous phase (if exists)
	switch (g_phase) {
		case OPTIONS_PHASE:
		case CREDITS_PHASE:
		case CREDIT_PHASE:
		case TITLE_PHASE:
		case MENU_PHASE:
			g_levelnum = 1;
		case LOADING_PHASE:
			OutputDebugString("Deleting global SCRBMP object\n");
			SafeDelete(g_ScrBmp); break;
		case GAME_PHASE:
			g_Midi->Stop();
			g_Midi->LoadMidiFromFile("resource/sounds/intro.mid",FALSE);
			g_Midi->Play();
			Cleanup_Level(g_levelnum); //g_ObjMgr->reset();
			break;
		default: break;
	}
	g_subphase=NONE; g_Time.UnPause();	g_ObjMgr->reset();
	switch (new_phase) {
		case CREDIT_PHASE:
			g_Midi->SetMasterVolume(-400);
			g_Midi->LoadMidiFromFile("resource/sounds/intro.mid",FALSE);
			g_Midi->Play();
			OutputDebugString("Creating credits.png SCRBMP object\n");
			g_ScrBmp = new CObjBmp("resource/screens/credits.png",0,TRUE); break;
		case TITLE_PHASE:
			OutputDebugString("INIT LEVELS\n");
			g_Midi->SetMasterVolume(-400);
			Init_Level(0);
			g_Time.Reset();
			g_ScrBmp = new CObjBmp("resource/screens/title.png"); break;
		case CREDITS_PHASE:
			g_Midi->SetMasterVolume(-400);
			g_ScrBmp = new CObjBmp("resource/screens/credit.png",0,TRUE); break;
		case OPTIONS_PHASE:
			g_Midi->SetMasterVolume(-400);
			g_ScrBmp = new CObjBmp("resource/screens/options.png",0,TRUE); break;
		case LOADING_PHASE:
			g_Midi->SetMasterVolume(-400);
			g_ScrBmp = new CObjBmp("resource/screens/loading.png"); break;
		case MENU_PHASE:
			g_Midi->SetMasterVolume(-400);
			g_ScrBmp = new CObjMenu; break;
		case GAME_PHASE:
			g_ScrBmp = new CObjBmp(NULL);
			g_ObjMgr->reset();
			Init_Level(g_levelnum); 
			Start_Level(g_levelnum); 
			g_Midi->SetMasterVolume(-900);
			break;
		default: break;
	}
	OutputDebugString("Resetting phase timer...\n");
	g_PhaseTimer.Reset();
	g_phase = new_phase;
	OutputDebugString("Switch Phase Complete...\n");
}

void Process_Phase(void)
{
	CObjBmp *t_bmp = NULL;
	D3DXVECTOR2 testvc(40,40);
    g_Time.UpdateClock();

	switch (g_phase)
	{
	case CREDIT_PHASE:
			g_D3DInput->GetInput((CObjBmp *) g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 6000 || g_ScrBmp->done())
				Switch_Phase(TITLE_PHASE);
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case TITLE_PHASE:
			g_D3DInput->GetInput((CObjBmp *)g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 6000 || g_ScrBmp->done())
				Switch_Phase(MENU_PHASE);
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case CREDITS_PHASE:
			g_D3DInput->GetInput((CObjBmp *)g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 12000 || g_ScrBmp->done())
				Switch_Phase(MENU_PHASE);
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case OPTIONS_PHASE:
			g_D3DInput->GetInput((CObjBmp *)g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 12000 || g_ScrBmp->done())
				Switch_Phase(MENU_PHASE);
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case MENU_PHASE:
		    g_SndMgr->beginframe();
			g_D3DInput->GetInput((CObjMenu *)g_ScrBmp);					 
			if (g_ScrBmp->done())
			{
				CObjMenu *tmenu = (CObjMenu *)g_ScrBmp;
				Switch_Phase(tmenu->nextphase());
				tmenu = NULL; // Paranoia
			} 
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
			//g_D3DObject->PaintText();
				g_D3DObject->EndPaint();		
			}
			break;
	case LOADING_PHASE:
			g_D3DInput->GetInput((CObjBmp *)g_ScrBmp);					 
			if (g_PhaseTimer.PeekTime() > 1000 || g_ScrBmp->done())
				Switch_Phase(GAME_PHASE);
			else {
				g_D3DObject->BeginPaint();
				g_ScrBmp->paint();
				g_D3DObject->EndPaint();		
			}
			break;
	case GAME_PHASE:
	    g_SndMgr->beginframe();
		g_D3DInput->GetInput((CObjBmp *)g_ScrBmp);
		if (g_ScrBmp->done()) {
			if(g_subphase == (LEVELUP)) {
				g_levelnum++;
				Switch_Phase(LOADING_PHASE);
			} else {
				Switch_Phase(MENU_PHASE);
			}
		   break;
		}
		if (g_subphase == NONE) {
			g_D3DInput->GetInput(g_hero);
		    g_D3DInput->GetInput(g_hero2);
			g_D3DInput->GetInput((CObjBmp *)g_ScrBmp);
			//if (g_ScrBmp->done()) Switch_Phase(QUIT);
			g_ObjMgr->spawn();
			g_ObjMgr->coldet();
			g_ObjMgr->move();
		}
		else {
			//Switch_Phase(MENU_PHASE);
			g_Time.Pause();
			if(g_Time.CmpTimeRaw(4000)) {
				g_Time.UnPause();
				t_bmp = (CObjBmp *)g_ScrBmp;
				t_bmp->SetDone(TRUE);
				t_bmp = NULL;
			}
//			g_Time.setInterval(4000);
//			if (g_Time.CmpTime()) Switch_Phase(MENU_PHASE);
		}
	    g_D3DObject->BeginPaint();
		g_ObjMgr->paint();
		if (g_subphase == GAME_OVER)
			g_ObjMgr->DrawText("Game Over",235,270);
		else if (g_subphase == WINNER)
			g_ObjMgr->DrawText("You Win",285,270);
		else if (g_subphase == LEVELUP)
			g_ObjMgr->DrawText("level complete",182,270);
		else if (g_Time.Paused())
			g_ObjMgr->DrawText("Paused",320,270);
		g_D3DObject->EndPaint();
		if (CHero::GetLives() == 0 && CHero2::GetLives() == 0) {
			g_subphase = GAME_OVER;
			g_Time.Reset();
		}
		if (g_subphase == NONE) {
			if (g_numenemies==0 && g_levelnum==3) {
				g_subphase = WINNER;
				g_Time.Reset();
				g_SndMgr->play(  N_WIN_NOISE);
			}
			else if (g_numenemies == 0) {
				g_subphase = LEVELUP;
				g_Time.Reset();
				g_SndMgr->play(  N_LEVELCOMPLETE);
			}
		}
		break;
	case QUIT:
		break;
	default:
		break;
	}

}