#include "StdAfx.h"
#include "world.h"

int CWorld::m_height;
int CWorld::m_scr_height;
int CWorld::m_scr_width;
int CWorld::m_width;
int CWorld::m_speed_x;
int CWorld::m_speed_y;
int CWorld::m_init;
float CWorld::m_cur_x;
float CWorld::m_cur_y;

CWorld::CWorld(void)
{
	if (!m_init)
		_Init();
}

CWorld::~CWorld(void)
{
}

BOOL CWorld::IsWayLeft(D3DXVECTOR2 *p, D3DXVECTOR2 *t)
{
  if (p->x+t->x < m_cur_x) return TRUE;
  else return FALSE;
}

BOOL CWorld::ToScreen(D3DXVECTOR2 *p, D3DXVECTOR2 *t)
{
   p->x -= m_cur_x;
   p->y -= m_cur_y;
//   static int cx = 0;
//   if (cx++ < 1000) {
//   char buff[500];
//   sprintf(buff,"Checking (x,y) to (curx,cury) : (%.2f,%.2f) (%.2f,%.2f) (%.2f,%.2f)\n",p->x,p->y,m_cur_x,m_cur_y,t->x,t->y);
//   OutputDebugString(buff);
//   }
   if ( (p->x > (0-t->x)) && (p->x < m_scr_width) )
	   return TRUE;
   return FALSE;
}

void CWorld::move(void)
{
   int dlt = m_timer.GetTime();
   m_cur_x += dlt * m_speed_x/1000;
   m_cur_y += dlt * m_speed_y/1000;
}

void CWorld::_Init()
{
	m_init = 1;

	m_scr_width = WIDTH;
	m_scr_height = HEIGHT;
	
	m_width = 400000;
	m_height = 600;

	m_speed_x = 10;
	m_speed_y = 0;

	m_cur_x = 0;
	m_cur_y = 0;
}
