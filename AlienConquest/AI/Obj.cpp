#include "StdAfx.h"
#include "obj.h"
#include "math.h"

const float CObj::SSCALE=(float)0.003;
const float CObj::ASCALE=(float)0.0014;

CObj::CObj(void)
{
	m_max_x=(float)50.2;
	m_max_y=(float)45.5;
	m_time.GetTime();
	m_z = 0;
	m_curtexture = NULL;
}

CObj::~CObj(void)
{
}

void CObj::paint(void)
{
	D3DXVECTOR2 pnt;
	pnt.x = m_dpos_x;
	pnt.y = m_dpos_y;

	if (m_curtexture)
			m_curtexture->Paint(&pnt);
}

void CObj::move(void)
{
	float tm = (float)m_time.GetTime()/1000;

//	char buff[500];
	//sprintf(buff,"Moving tm=%f\n",tm);
	//OutputDebugString(buff);
	//sprintf(buff,"Old speed is x,y=%.2f,%.2f -  %.4f %.4f\n",m_speed_x,m_speed_y,
	//	m_accel_x,m_accel_y);
	//OutputDebugString(buff);

	// Calculate Distance
	float dlt_x = (float)(m_speed_x*tm + 0.5*m_accel_x*tm*tm);
	float dlt_y = (float)(m_speed_y*tm + 0.5*m_accel_x*tm*tm);
	//sprintf(buff,"Delta x,y=%.2f,%.2f - %.2f, %.2f\n",dlt_x,dlt_y,
	//	m_dpos_x, m_dpos_y
	//	);
	//OutputDebugString(buff);

	// Take into account acceleration
	m_dpos_x += dlt_x;
	m_dpos_y += dlt_y;
	float MARGIN_X = 50;
	float MARGIN_Y = 50;
	if (m_dpos_x < (0-MARGIN_X)) m_dpos_x = WIDTH;
	if (m_dpos_y < (0-MARGIN_Y)) m_dpos_y = HEIGHT;
	if (m_dpos_x > WIDTH) m_dpos_x = 0-MARGIN_X;
	if (m_dpos_y > HEIGHT) m_dpos_y = 0-MARGIN_Y;
	//m_pos_x = (int) m_dpos_x % WIDTH;
	//m_pos_y = (int) m_dpos_y % HEIGHT;
	//sprintf(buff,"X,Y = %i,%i\n",m_pos_x, m_pos_y);
	//OutputDebugString(buff);

	// New speed
	m_speed_x += m_accel_x*tm;
	m_speed_y += m_accel_y*tm;

	if (fabs(m_speed_x) > m_max_x) m_accel_x=0;
	if (fabs(m_speed_y) > m_max_y) m_accel_y=0;
}

void CObj::SetSpeed(float xspeed,float yspeed)
{
	m_speed_x = xspeed;
	m_speed_y = yspeed;
}

void CObj::SetAccel(float xaccel,float yaccel)
{
	m_accel_y = yaccel;
	m_accel_x = xaccel;
}

void CObj::SetPosition(float x, float y)
{
	m_dpos_x = x;
	m_dpos_y = y;
}

void CObj::accel(float xdelta, float ydelta)
{
  m_accel_x+=xdelta;
  m_accel_y+=ydelta;
}