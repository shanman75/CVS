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
#include "Terrain.h"
#include "GameState.h"

CTimer g_time;
CTimer time, time2;
cTerrain *terrain;
cCamera cam;

void g_MainDestroy()
{
  delete terrain;
  delete g_GameState;
}
void g_MainInit()
{
   g_GameState = new cGameState;
   g_ObjMgr->reset();
   g_ObjMgr->SetCurrentCamera(new cCamera);
   g_GameState->AddPlayer();

   D3DLIGHT9 light;
   ZeroMemory(&light,sizeof(light));
   D3DXVECTOR3 pos (0,400,0);
   //g_D3DObject->m_d3ddevice9->GetLight(0,&light);
   light.Position = pos;
   light.Direction = D3DXVECTOR3 (0,-1,0);
   light.Diffuse.g = light.Diffuse.b =   light.Diffuse.r = 1.0f;
   light.Specular = light.Diffuse;
   light.Diffuse.r = 0.9f;
   light.Range = 500;
   light.Type = D3DLIGHT_DIRECTIONAL;
   g_D3DObject->m_d3ddevice9->SetLight(0,&light);
   g_D3DObject->m_d3ddevice9->LightEnable(0,true);

   g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_AMBIENT,     D3DCOLOR_RGBA(250,250,250,0) );
   //float clip = 500.0f;
   //g_D3DObject->m_d3ddevice9->SetClipPlane(0,&clip);
   //g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_AMBIENT,     D3DCOLOR_RGBA(15,15,15,0) );

   terrain = new cTerrain();
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
   //g_D3DInput->GetInput((cTerrain *)terrain);
   g_GameState->GetInput();
 
   g_ObjMgr->move();

 
   g_D3DObject->BeginPaint();
   g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,0,50),debg);
   
   g_ObjMgr->GetCurrentCamera()->SetCamera();
   g_ObjMgr->paint();
 

   D3DMATERIAL9 d3dMaterial;
   memset(&d3dMaterial, 0, sizeof(d3dMaterial));
   d3dMaterial.Diffuse.r = 
   d3dMaterial.Diffuse.g = 
   d3dMaterial.Diffuse.b = 
   d3dMaterial.Diffuse.a = 1.0f;
   d3dMaterial.Specular = d3dMaterial.Diffuse;
   d3dMaterial.Diffuse.g = 1.0f;
   d3dMaterial.Power = 1.0f; // arbitrary

   g_D3DObject->m_d3ddevice9->SetMaterial( &d3dMaterial );
   terrain->Paint();

   g_D3DObject->EndPaint();
}
