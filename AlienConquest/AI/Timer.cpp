#include "StdAfx.h"
#include "timer.h"
#include <Mmsystem.h>

DWORD CTimer::m_gtime = GetTickCount();
BOOL CTimer::m_pause = FALSE;
int CTimer::m_dltime = 0;

CTimer::CTimer(void)
{
//	m_numover = 0;
	Reset();
}

CTimer::~CTimer(void)
{
}

void CTimer::UpdateClock()
{
	if (m_dltime>0) m_dltime--;
	if (m_pause) return;
	m_gtime = GetTickCount(); 
	//m_gtime = timeGetTime();
}

DWORD CTimer::PeekTime()
{
  //return m_ltime > my_gtime ? m_ltime-my_gtime : ((DWORD)(0 - 1)) - m_ltime + m_gtime;
  if (m_dltime>0) { Reset(); return 0; }
  return abs((m_gtime-m_ltime)%TIMER_MAXDLTA);
}

DWORD CTimer::UpdatePeekTime()
{
  //return m_ltime > my_gtime ? m_ltime-my_gtime : ((DWORD)(0 - 1)) - m_ltime + m_gtime;
  UpdateClock();
  return PeekTime();
}

DWORD CTimer::GetTime()
{
  UINT my_time = PeekTime();
  Reset();
  return my_time;
}

DWORD CTimer::UpdateGetTime()
{
   UpdateClock();
   return GetTime();
}

void CTimer::Reset()
{
	m_ltime = m_gtime;
}

void CTimer::Pause()
{
   m_pause = TRUE;
}

void CTimer::TogglePause()
{ 
	if (m_pause) m_dltime = 2;
	m_pause = !m_pause;
}

BOOL CTimer::CmpTime(DWORD cmptm)
{
  BOOL rtval = (PeekTime() >= cmptm); 
  if (rtval) Reset(); 
  return rtval;
}

BOOL CTimer::CmpTime(void)
{
   return CmpTime(m_timeinterval);
}

void CTimer::setInterval(DWORD interval)
{
	m_timeinterval = interval;
}

void CTimer::UnPause(void)
{
	if (m_pause) m_dltime = 2;
	m_pause = FALSE;
}

BOOL CTimer::Paused(void)
{
	return m_pause;
}

BOOL CTimer::CmpTimeRaw(DWORD cmptm)
{
  DWORD mytime = GetTickCount();
  DWORD interval = abs((mytime-m_ltime)%TIMER_MAXDLTA);

  BOOL rtval = (interval >= cmptm); 
  if (rtval) m_ltime = mytime;
  return rtval;
}

BOOL CTimer::CmpTimeRaw(void)
{
	return CmpTimeRaw(m_timeinterval);
}