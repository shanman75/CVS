// TankGWAR.cpp : Defines the entry point for the application.
//
// BLAH

#include "stdafx.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "D3DObject.h"
#include "D3DInput.h"
#include "Texture.h"
#include "ObjMgr.h"
#include "cModel.h"
#include "TankGWAR.h"
#include "MainWnd.h"
#include "Timer.h"
#include "Camera.h"
#include "CameraBehindTank.h"
#include "GameState.h"

CTimer g_time;
CTimer time, time2;
bool sh_FPS = false;

void g_MainDestroy()
{
  SAFE_DELETE(g_GameState);
}
void g_MainInit()
{
   g_GameState = new cGameState;
   //g_GameState = NULL;
   g_ObjMgr->reset();
//   g_GameState->AddPlayer();

}


void g_MainGameLoop() 
{
   static int tm = 1;
   static char debg[255];
   if (--tm == 0) {
     sprintf (debg,"FPS %.2f\n",4000 / time.GetTime());
     //OutputDebugString(debg);
	   tm = 4;
   }

   g_time.UpdateClock();
   if (g_D3DObject->DeviceLost())
     return;

   g_GameState->GetInput();
 
   g_ObjMgr->move();
   g_GameState->move();

   g_D3DObject->BeginPaint();
   g_GameState->GetCurrentCamera()->SetCamera();
   g_GameState->paintbg();
   if (sh_FPS)
   g_D3DObject->DrawTextStr(50,530,D3DCOLOR_XRGB(240,0,50),debg);
   
   g_ObjMgr->paint();
   g_GameState->paint();
 
   g_D3DObject->EndPaint();
}
