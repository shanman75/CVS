#include "StdAfx.h"
#include "timer.h"

DWORD CTimer::m_gtime = GetTickCount();

CTimer::CTimer(void)
{
	Reset();
}

CTimer::~CTimer(void)
{
}

void CTimer::UpdateClock()
{
	m_gtime = GetTickCount(); 
}

DWORD CTimer::PeekTime()
{
  //return m_ltime > my_gtime ? m_ltime-my_gtime : ((DWORD)(0 - 1)) - m_ltime + m_gtime;
  return (m_gtime-m_ltime)%TIMER_MAXDLTA;
}

DWORD CTimer::UpdatePeekTime()
{
  //return m_ltime > my_gtime ? m_ltime-my_gtime : ((DWORD)(0 - 1)) - m_ltime + m_gtime;
  UpdateClock();
  return PeekTime();
}

DWORD CTimer::GetTime()
{
  UINT my_ltime = m_ltime;
  Reset();
  //return m_ltime > my_gtime ? my_gtime-m_ltime : ((DWORD)(0 - 1)) - m_ltime + m_gtime;
  return (m_gtime-my_ltime)%TIMER_MAXDLTA;
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

