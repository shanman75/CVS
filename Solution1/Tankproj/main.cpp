/************************************************************************
Main.cpp

Contains the windows main, and windows procedure functions that control
the flow of the game.

Last Updated: April 8,2004

************************************************************************/

//all includes used throughout the progam included here
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
//#include <malloc.h>
#include <windows.h>
#include <windowsx.h>
#include "input.h"
#include "mp3aud.h"
#include "terrain.h"
#include "initialization.h"
#include "d3dmat.h"
#include "destroy.h"

BOOL active;
bool cameraontanks=true;  //boolean for free floating camera
bool showfps=false;       //boolean for displaying framerate
CInputManager* InputManager; //input manager
mp3stream *mp3_1;

IDirect3D9 *d3dcomobj=NULL;
IDirect3DDevice9 *d3ddevice=NULL;
D3DPRESENT_PARAMETERS presparams;
//IDirect3DSurface9* d3dbackbuffer=NULL;
//IDirect3DSurface9* testscreen=NULL;
LPDIRECT3DTEXTURE9 BackgroundTexture=NULL; //texture for background
LPDIRECT3DTEXTURE9 skytexture=NULL; //texture for background
LPDIRECT3DVERTEXBUFFER9 sbnegx;
LPDIRECT3DVERTEXBUFFER9 sbposx;
LPDIRECT3DVERTEXBUFFER9 sbnegz;
LPDIRECT3DVERTEXBUFFER9 sbposz;
LPDIRECT3DVERTEXBUFFER9 sbtop;

//add texture variables?

LRESULT CALLBACK WindowProc(HWND hwnd,UINT message, WPARAM wparam,LPARAM lparam)
{	switch(message)
    { case WM_ACTIVATEAPP: active=(BOOL)wparam;
                         break;
	  case WM_DESTROY:	 delete InputManager; //ditto for input manager
		                 delete mp3_1;
						 CoUninitialize();
		                 clean();
						 break;
	  case WM_USER:      mp3_1->OnGraphEvent();		// handles events
                         break;    
	  default:			 return DefWindowProc(hwnd,message,wparam,lparam);
	} //switch
	return 0L;
}	//winproc

int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR cmdline,int cmdshow)
{	MSG message;
	HWND hwnd;
	hwnd=CreateDefaultWindow("Tank Battle",hinstance,cmdshow);
	if(FAILED(hwnd))
		return false;
	if(!initd3d(hinstance,hwnd))
		return false;
	CoInitialize(NULL);
	mp3_1=new mp3stream(hwnd);
    mp3_1->CreateGraph("back.mp3");
	setupd3d();
	if(!LoadTextures())return FALSE;
    //create input manager (create after graphics)
    InputManager=new CInputManager(hinstance,hwnd);
	while(true)
		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{  if(!GetMessage(&message,NULL,0,0))
		      return (int)message.wParam;
	       TranslateMessage(&message);
		   DispatchMessage(&message);
		}
		else if(active)
		{	mp3_1->startsound();
			ProcessFrame();
		}	//else if
		else
		{	mp3_1->stopsound();  //stop the sound when alt-tab out
			WaitMessage();
		}	//else
}