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
#include "Terrain.h"
#include "SkyBox.h"
#include "GameState.h"

CTimer g_time;
CTimer time, time2;
cTerrain *terrain;
cSkyBox *skybox;
cCamera cam;

void g_MainDestroy()
{
  delete terrain;
  delete skybox;
  delete g_GameState;
}
void g_MainInit()
{
   g_GameState = new cGameState;
   g_ObjMgr->reset();
   g_GameState->SetCurrentCamera(new cCameraBehindTank);
   g_GameState->AddPlayer();



   g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_AMBIENT,     D3DCOLOR_RGBA(180,180,180,255) );
   //float clip = 500.0f;
   //g_D3DObject->m_d3ddevice9->SetClipPlane(0,&clip);
   //g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_AMBIENT,     D3DCOLOR_RGBA(15,15,15,0) );

   terrain = new cTerrain();
   skybox = new cSkyBox();
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

   //g_D3DInput->GetInput(g_ObjMgr->GetCurrentCamera());					 
   g_D3DInput->GetInput((cTerrain *)terrain);
   g_GameState->GetInput();
 
   g_ObjMgr->move();

   g_D3DObject->BeginPaint();
   g_GameState->GetCurrentCamera()->SetCamera();
   skybox->Paint();
   terrain->Paint();
   g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,0,50),debg);
   
   g_ObjMgr->paint();
 
   g_D3DObject->EndPaint();
}
