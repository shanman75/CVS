#include "StdAfx.h"
#include "obj.h"

const double CObj::SSCALE=0.012;
const double CObj::ASCALE=0.012;

CObj::CObj(void)
{
	m_max_x=5;
	m_max_y=2;
	m_time.GetTime();
}

CObj::~CObj(void)
{
}

void CObj::paint(void)
{

}

void CObj::move(void)
{
	int tm = m_time.GetTime();

	char buff[500];
	sprintf(buff,"Moving tm=%i\n",tm);
	OutputDebugString(buff);

	sprintf(buff,"Old speed is x,y=%.2f,%.2f -  %.4f %.4f\n",m_speed_x,m_speed_y,m_accel_x,ASCALE);
	OutputDebugString(buff);
	// Take into account acceleration
	m_speed_x += (m_accel_x*tm*ASCALE);
	m_speed_y += (m_accel_y*tm*ASCALE);
	m_speed_x = m_speed_x > m_max_x ? m_max_x : m_speed_x;
	m_speed_x = m_speed_x < (0-m_max_x) ? 9-m_max_x : m_speed_x;
	m_speed_y = m_speed_y > m_max_y ? m_max_y : m_speed_y;
	m_speed_y = m_speed_y < (0-m_max_y) ? 0-m_max_y : m_speed_y;
//	sprintf(buff,"New speed is x,y=%.2f,%.2f\n",m_speed_x,m_speed_y);
//	OutputDebugString(buff);

	// Calculate next x,y value
	m_dpos_x += m_speed_x*tm*SSCALE;
	m_dpos_y += m_speed_y*tm*SSCALE;
	m_dpos_x += m_dpos_x < 0 ? WIDTH : 0;
	m_dpos_y += m_dpos_y < 0 ? HEIGHT : 0;
	m_pos_x = (int) m_dpos_x % WIDTH;
	m_pos_y = (int) m_dpos_y % HEIGHT;
//	m_pos_x = m_pos_x > WIDTH ? 0 : m_pos_x;
//	m_pos_y = m_pos_y > HEIGHT ? 0 : m_pos_y;

}

void CObj::SetSpeed(double xspeed,double yspeed)
{
	m_speed_x = xspeed;
	m_speed_y = yspeed;
}

void CObj::SetAccel(double xaccel,double yaccel)
{
	m_accel_y = yaccel;
	m_accel_x = xaccel;
}

void CObj::SetPosition(double x, double y)
{
	m_dpos_x = x;
	m_dpos_y = y;
}
