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

BOOL CWorld::ToScreen(D3DXVECTOR2 *p, D3DXVECTOR2 *t)
{
   p->x -= m_cur_x;
   p->y -= m_cur_y;
   if ( ( (p->x > (-t->x)) && (p->x < m_scr_width) ) &&
	    ( (p->y > (-t->y)) && (p->y < m_scr_height) ) )
	   return TRUE;
   return FALSE;
}

void CWorld::move(void)
{
   int dlt = m_timer.GetTime();
   m_cur_x += dlt * m_speed_x /1000;
   m_cur_y += dlt * m_speed_y/1000;
}

void CWorld::_Init()
{
	m_init = 1;

	m_scr_width = WIDTH;
	m_scr_height = HEIGHT;
	
	m_width = 4000;
	m_height = 600;

	m_speed_x = 5;
	m_speed_y = 0;

	m_cur_x = 0;
	m_cur_y = 0;
}