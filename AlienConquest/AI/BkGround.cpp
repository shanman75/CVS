#include "StdAfx.h"
#include "bkground.h"

extern int g_levelnum;

int CBkGround::m_graph_init = 0;
CTexture *CBkGround::m_top[3];
CTexture *CBkGround::m_middle[3];
CTexture *CBkGround::m_bottom[3];

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
   m_middle[0] = new CTexture("resource/backgrounds/midground.png",0xFFFF00FF,NULL,256,256);
   m_bottom[0] = new CTexture("resource/backgrounds/sea.png",0xFFFF00FF,NULL,256,256);

   char tpath[500];
   sprintf (tpath,"resource/backgrounds/background%i.png",g_levelnum);
   m_top[0] = new CTexture(tpath,0xFFFF00FF,NULL,256,256);
  // m_top[1] = new CTexture("resource/backgrounds/background2.png",0xFFFF00FF,NULL,256,256);
  // m_top[2] = new CTexture("resource/backgrounds/background3.png",0xFFFF00FF,NULL,256,256);
}

void CBkGround::_UnloadGraphics(void)
{
//	delete m_regular[0];
//	delete m_regular[1];
//	delete m_regular[2];

	delete m_top[0];
	//delete m_top[1];
	//delete m_top[2];

	delete m_bottom[0];
	delete m_middle[0];
}

void CBkGround::paint(void)
{
	D3DXVECTOR2 trans(0,0);
	RECT SrcRect;

	trans.x=0;

	int w = g_D3DObject->GetWidth();
	int h = g_D3DObject->GetHeight();
	float m_x = m_world.GetCurX();

	static float sky = (float) 1.0;
	static float groud =(float) 2.0;
	static float below =(float) 4.0;

	trans.x = 0;
	trans.y = 0;
	SetRect(&SrcRect,(int)(m_x*sky),0,(int)(m_x*sky+w),600);
	m_top[0]->Paint(&SrcRect,&trans);
	SetRect(&SrcRect,(int)(m_x*groud),0,(int)(m_x*groud+w),600);
	m_middle[0]->Paint(&SrcRect,&trans);
	SetRect(&SrcRect,(int)(m_x*below),0,(int)(m_x*below+w),600);
	m_bottom[0]->Paint(&SrcRect,&trans);
}