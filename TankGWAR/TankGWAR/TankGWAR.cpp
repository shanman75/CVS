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
#include "3DObjectMissile.h"

CTimer g_time;
CTimer time, time2;
CTexture *tex1;
cTerrain *terrain;
cModel *model, *model2, *model3;
cCamera cam;

void g_MainDestroy()
{
  delete tex1;
  delete model;
  delete model2;
  delete model3;
  delete terrain;
}
void g_MainInit()
{
   g_ObjMgr->reset();
   //c3DObjectMissile *myobj = new c3DObjectMissile();
   //g_ObjMgr->add(myobj);
   tex1 = new CTexture("resource\\blah.png",0xFFFF00FF);
   model = new cModel(0,0,0);
   model2 = new cModel(0,0,50);
   model3 = new cModel(30,0,-10);

   D3DLIGHT9 light;
   ZeroMemory(&light,sizeof(light));
   D3DXVECTOR3 pos (0,40,0);
   //g_D3DObject->m_d3ddevice9->GetLight(0,&light);
   light.Position = pos;
   light.Direction = D3DXVECTOR3 (0,-1,0);
   light.Diffuse.g = light.Diffuse.b =   light.Diffuse.r = 1.0f;
   //light.Diffuse.r = 0.2f;
   //light.Ambient = light.Diffuse;
   light.Specular = light.Diffuse;
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
   static CTimer bull;
   c3DObjectMissile *myobj;

   bull.setInterval(100);
   if (bull.CmpTime()) {
     g_ObjMgr->add(new c3DObjectMissile());
   }


   g_time.UpdateClock();
   g_time.UnPause();

   g_D3DInput->GetInput((cCamera *)&cam);					 
   g_D3DInput->GetInput((cTerrain *)terrain);
   g_ObjMgr->move();

   if (--tm == 0) {
     sprintf (debg,"FPS %.2f\n",4000 / time.GetTime());
     //OutputDebugString(debg);
	 tm = 4;
   }
 
   g_D3DObject->BeginPaint();
   //g_D3DObject->DrawTextStr((int)(time2.PeekTime()/50)%800,100,D3DCOLOR_XRGB(240,0,50),"Hello World!!!");
   g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,0,50),debg);
   

   cam.SetCamera();
   g_ObjMgr->paint();
 
   D3DMATERIAL9 d3dMaterial;
   memset(&d3dMaterial, 0, sizeof(d3dMaterial));
   d3dMaterial.Diffuse.r = 1.0f;
   d3dMaterial.Diffuse.g = 1.0f;
   d3dMaterial.Diffuse.b = 1.0f;
   d3dMaterial.Diffuse.a = 1.0f;
//   d3dMaterial.Ambient = d3dMaterial.Diffuse;
   d3dMaterial.Specular = d3dMaterial.Diffuse;
   d3dMaterial.Power = 100.0f; // arbitrary

   g_D3DObject->m_d3ddevice9->SetMaterial( &d3dMaterial );

   model->Paint();
   terrain->Paint();

   g_D3DObject->m_pd3dxSprite->Begin(0);
   //tex1->Paint(fmod(time2.PeekTime()/50,800),fmod((time2.PeekTime()/50),600));
   g_D3DObject->m_pd3dxSprite->End();

   g_D3DObject->EndPaint();
}
