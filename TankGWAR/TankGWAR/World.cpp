#include "StdAfx.h"
#include "world.h"

int CWorld::m_height;
int CWorld::m_scr_height;
int CWorld::m_scr_width;
int CWorld::m_width;
float CWorld::m_speed_x;
float CWorld::m_speed_y;
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
  if (p->x+t->x < m_cur_x-10) return TRUE;
  else return FALSE;
}

BOOL CWorld::IsOnScreen(D3DXVECTOR2 *p, D3DXVECTOR2 *t)
{
  if (p->x+t->x < m_cur_x && p->x+t->x > m_cur_x+800) return FALSE;
  else return TRUE;
}

BOOL CWorld::IsOnScreen(int x, int y, int width, int height)
{
	D3DXVECTOR2 t ((float)x,(float)y);
	D3DXVECTOR2 p ((float)width,(float)height);
	return IsOnScreen(&t,&p);
}

BOOL CWorld::ToScreen(D3DXVECTOR2 *p, D3DXVECTOR2 *t)
{
   p->x -= m_cur_x;
   p->y -= m_cur_y;
   if ( (p->x > (0-t->x)) && (p->x < m_scr_width) )
	   return TRUE;
   return FALSE;
}

void CWorld::move(void)
{
   float dlt = m_timer.GetTime();
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

	m_speed_x = 50;
	m_speed_y = 0;

	m_cur_x = 0.0f;
	m_cur_y = 0.0f;

	m_timer.Reset();
}

void CWorld::reset()
{
	_Init();
}