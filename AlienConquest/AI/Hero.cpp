#include "StdAfx.h"
#include "hero.h"

int CHero::m_graph_init = 0;
CTexture *CHero::m_regular[1];

CHero::CHero(void)
{
  CObj();
  if (!m_graph_init) _LoadGraphics();
    m_speed_x=50;
	m_state = REGULAR;
	m_max_x= 180;
	m_max_y= 170;
	m_time.GetTime();
}

CHero::~CHero(void)
{
	_UnloadGraphics();
}

void CHero::_LoadGraphics(void)
{
   RECT trect;
   SetRect(&trect,1,1,126,97);
   OutputDebugString("Loading Hero graphics\n");
   m_regular[0] = new CTexture("resource/herodumdum2.bmp",0xFFFF00FF,NULL);
}

void CHero::_UnloadGraphics(void)
{
	delete m_regular[0];
}

void CHero::paint(void)
{
	D3DXVECTOR2 pnt;
	pnt.x = m_dpos_x;
	pnt.y = m_dpos_y;
	switch (m_state) {
		case REGULAR:
		default:
			if (m_regular[0] != NULL)
				m_regular[0]->Paint(&pnt);
			break;
	};
}

void CHero::move(void)
{
	CObj::move();
	return;

	float X_SCALE = (float)1.6;
	float Y_SCALE = (float)1.9;
	if (m_speed_x > X_SCALE) m_speed_x-=X_SCALE;
	else if (m_speed_x < -X_SCALE) m_speed_x += X_SCALE;
	else m_speed_x += m_speed_x > 0.0 ? (float)-0.1: (float)0.1;

	if (m_speed_y > Y_SCALE) m_speed_y-=Y_SCALE;
	else if (m_speed_y < -Y_SCALE) m_speed_y += Y_SCALE;
	else m_speed_y += m_speed_y > 0.0 ? (float)-0.1: (float)0.1;
}