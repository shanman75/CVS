// TankGWAR.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TankGWAR.h"
#include "MainWnd.h"
#include "Timer.h"
#include <stdio.h>

#include "D3DObject.h"
#include "Texture.h"

CTimer global;
CTimer time, time2;
CTexture *tex1;

void g_MainDestroy()
{
  delete tex1;
}
void g_MainInit()
{
tex1 = new CTexture("blah.png",0xFFFF00FF);
}

void g_MainGameLoop() 
{
   static int tm = 1;
   static char debg[255];
  
   global.UpdateClock();

   g_D3DObject->BeginPaint();
   sprintf (debg,"Blah %f\n",time.GetTime());
   OutputDebugString(debg);

   g_D3DObject->DrawTextStr((int)(time2.PeekTime()/50)%800,100,D3DCOLOR_XRGB(240,0,50),"Hello World!!!");
   g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,0,50),debg);
   tex1->Paint((int)(time2.PeekTime()/50)%800,(int)(time2.PeekTime()/50)%600);

   g_D3DObject->EndPaint();
}
