#include "StdAfx.h"
#include "hero.h"

int CHero::m_graph_init = 0;
CTexture *CHero::m_regular[1];

CHero::CHero(void)
{
  CObj();
  if (!m_graph_init) _LoadGraphics();
	m_state = REGULAR;
	m_max_x= 320;
	m_max_y= 320;
	SetAccel(0,0);
	SetSpeed(0,0);
	m_time.GetTime();
}

CHero::~CHero(void)
{
	_UnloadGraphics();
//	~CObj();
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
	switch (m_state) {
		case REGULAR:
		default:
			if (m_regular[0] != NULL)
				m_curtexture = m_regular[0];
			break;
	};
	CObj::paint();
}

void CHero::move(void)
{
	if (m_mov_x.PeekTime() > 400) { m_speed_x=0; }
	if (m_mov_y.PeekTime() > 400) { m_speed_y=0; }
	CObj::move();
	return;

	/*
	float X_SCALE = (float)1.6;
	float Y_SCALE = (float)1.9;
	if (m_speed_x > X_SCALE) m_speed_x-=X_SCALE;
	else if (m_speed_x < -X_SCALE) m_speed_x += X_SCALE;
	else m_speed_x += m_speed_x > 0.0 ? (float)-0.1: (float)0.1;

	if (m_speed_y > Y_SCALE) m_speed_y-=Y_SCALE;
	else if (m_speed_y < -Y_SCALE) m_speed_y += Y_SCALE;
	else m_speed_y += m_speed_y > 0.0 ? (float)-0.1: (float)0.1;
	*/
}

void CHero::event(EVENT event)
{
	float spdx = 150.5;
	float spdy = 150.5;

	switch(event) {
		case UP:
			m_speed_y = -(sqrt(spdy*spdy-(m_speed_x*m_speed_x)/2)); 
			if (m_speed_x != 0) m_speed_x = (m_speed_x/fabs(m_speed_x))*fabs(m_speed_y);
			m_mov_y.Reset();
			break;
		case DOWN:
			m_speed_y = spdy; m_mov_y.Reset();
			break;
		case LEFT:
			m_speed_x = -spdx; m_mov_x.Reset();
			break;
		case RIGHT:
			m_speed_x = spdx; m_mov_x.Reset();
			break;
		default:
			break;
	}
}