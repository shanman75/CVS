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

CTimer time, time2;
CTexture *tex1;
cModel *model, *model2, *model3;
cCamera cam;


void g_MainDestroy()
{
  delete tex1;
  delete model;
  delete model2;
  delete model3;
}
void g_MainInit()
{
   g_ObjMgr.reset();
   CObj *myobj = new CObj();
   g_ObjMgr.add(myobj);
   tex1 = new CTexture("resource\\blah.png",0xFFFF00FF);
   model = new cModel();
   model2 = new cModel();
   model2->SetXYZ(0,0,5);
   model3 = new cModel();
   model3->SetXYZ(0,0,-5);
}


void g_MainGameLoop() 
{
   static int tm = 1;
   static char debg[255];

   time.UpdateClock();
   if (--tm == 0) {
     sprintf (debg,"FPS %.2f\n",4000 / time.GetTime());
     OutputDebugString(debg);
	 tm = 4;
   }


   
   g_D3DObject->BeginPaint();
   g_ObjMgr.paint();
   g_D3DObject->DrawTextStr((int)(time2.PeekTime()/50)%800,100,D3DCOLOR_XRGB(240,0,50),"Hello World!!!");
   g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,0,50),debg);
   
   g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_AMBIENT,        0x00FFFFFF );
   D3DMATERIAL9 d3dMaterial;
   memset(&d3dMaterial, 0, sizeof(d3dMaterial));
   d3dMaterial.Diffuse.r = 1.0f;
   d3dMaterial.Diffuse.g = 1.0f;
   d3dMaterial.Diffuse.b = 1.0f;
   d3dMaterial.Diffuse.a = 1.0f;
   d3dMaterial.Ambient = d3dMaterial.Diffuse;
   d3dMaterial.Specular = d3dMaterial.Diffuse;
   d3dMaterial.Power = 100.0f; // arbitrary

   g_D3DObject->m_d3ddevice9->SetMaterial( &d3dMaterial );

	g_D3DInput->GetInput((cCamera *)&cam);					 
	cam.SetCamera();
 
	model->Paint();
	model2->Paint();
	model3->Paint();

   g_D3DObject->m_pd3dxSprite->Begin(0);
   tex1->Paint((int)(time2.PeekTime()/50)%800,(int)(time2.PeekTime()/50)%600);
   g_D3DObject->m_pd3dxSprite->End();

   g_D3DObject->EndPaint();
}
