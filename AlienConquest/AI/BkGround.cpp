#include "StdAfx.h"
#include "bkground.h"

int CBkGround::m_graph_init = 0;
CTexture *CBkGround::m_regular[3];

CBkGround::CBkGround(void)
{
  CObj();
  m_z = 3;
  if (!m_graph_init++) _LoadGraphics();
}

CBkGround::~CBkGround(void)
{
	if (!--m_graph_init) _UnloadGraphics();
}

void CBkGround::_LoadGraphics(void)
{
   OutputDebugString("Loading Background graphics\n");
   m_regular[0] = new CTexture("resource/farsky.png",0xFFFF00FF,NULL,256,256);
   m_regular[1] = new CTexture("resource/middleground.png",0xFFFF00FF,NULL,256,256);
   m_regular[2] = new CTexture("resource/water.png",0xFFFF00FF);
}

void CBkGround::_UnloadGraphics(void)
{
	delete m_regular[0];
	delete m_regular[1];
	delete m_regular[2];
}

void CBkGround::paint(void)
{
	D3DXVECTOR2 trans(0,0);
	RECT SrcRect, *pSrcRect = &SrcRect;

	trans.x=0;

	int w = g_D3DObject->GetWidth();
	int h = g_D3DObject->GetHeight();
	static int m_x = 0;
	m_x += m_time_x.GetTime();

	float sky = (float) 0.012;
	float groud =(float) 0.065;
	float below =(float) 0.168;

	int lft = (((int)(m_x*sky))); //% tex[0]->GetWidth());
	trans.x = 0;
	trans.y = 0;
	SetRect(&SrcRect,lft,0,lft+w,m_regular[0]->GetHeight());
	m_regular[0]->Paint(&SrcRect,&trans);
	SetRect(&SrcRect,(int)(m_x*groud),0,(int)(m_x*groud+w),m_regular[1]->GetHeight());
	m_regular[1]->Paint(&SrcRect,&trans);
	SetRect(&SrcRect,(int)(m_x*below),0,(int)(m_x*below+w),m_regular[2]->GetHeight());
	trans.y = (float)(h-50);
	m_regular[2]->Paint(&SrcRect,&trans);
}