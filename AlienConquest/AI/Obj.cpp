#include "StdAfx.h"
#include "obj.h"

const double CObj::SSCALE=0.003;
const double CObj::ASCALE=0.0014;

CObj::CObj(void)
{
	m_max_x=50.2;
	m_max_y=45.5;
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
	double tm = (double)m_time.GetTime()/1000;

	char buff[500];
	//sprintf(buff,"Moving tm=%f\n",tm);
	//OutputDebugString(buff);
	//sprintf(buff,"Old speed is x,y=%.2f,%.2f -  %.4f %.4f\n",m_speed_x,m_speed_y,
	//	m_accel_x,m_accel_y);
	//OutputDebugString(buff);

	// Calculate Distance
	double dlt_x = m_speed_x*tm + 0.5*m_accel_x*tm*tm;
	double dlt_y = m_speed_y*tm + 0.5*m_accel_x*tm*tm;
	//sprintf(buff,"Delta x,y=%.2f,%.2f - %.2f, %.2f\n",dlt_x,dlt_y,
	//	m_dpos_x, m_dpos_y
	//	);
	//OutputDebugString(buff);

	// Take into account acceleration
	m_dpos_x += dlt_x;
	m_dpos_y += dlt_y;
	if (m_dpos_x < 0) m_dpos_x = WIDTH;
	if (m_dpos_y < 0) m_dpos_y = HEIGHT;
	m_pos_x = (int) m_dpos_x % WIDTH;
	m_pos_y = (int) m_dpos_y % HEIGHT;
	//sprintf(buff,"X,Y = %i,%i\n",m_pos_x, m_pos_y);
	//OutputDebugString(buff);

	// New speed
	m_speed_x += m_accel_x*tm;
	m_speed_y += m_accel_y*tm;

	if (abs(m_speed_x) > m_max_x) m_accel_x=0;
	if (abs(m_speed_y) > m_max_y) m_accel_y=0;

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

void CObj::accel(double xdelta, double ydelta)
{
  m_accel_x+=xdelta;
  m_accel_y+=ydelta;
}