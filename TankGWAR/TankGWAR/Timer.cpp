#include "StdAfx.h"
#include "timer.h"
//#include <Mmsystem.h>

/*
   static LARGE_INTEGER tck;
   static LARGE_INTEGER perfFreq;
   static float performanceTimerFrequency;

	if (!SUCCEEDED(::QueryPerformanceFrequency(&perfFreq))) {
		OutputDebugString("QueryPerformanceFrequency failed");
	}
	performanceTimerFrequency = (float)perfFreq.QuadPart;

     LARGE_INTEGER clock;
     QueryPerformanceCounter(&clock);
     LONGLONG diff = clock.QuadPart - tck.QuadPart;
     float timeStep = (float)diff / performanceTimerFrequency;
     sprintf(debg,"Counter is %i Frame/sec = %f\n", tm, 30/timeStep);
     tck = clock;
	 tm = 0;

*/

float CTimer::GetHiTime(void)
{
	if (m_perfFreq != 0) {
	 QueryPerformanceCounter(&m_largeInt);
	 return (float)   m_largeInt.QuadPart;
	}
	else 
		return 0;
     //sprintf(debg,"Counter is %i Frame/sec = %f\n", tm, 30/timeStep);
     //tck = clock;
}

float CTimer::TimeDelta()
{
	 LONGLONG diff = m_gtime.QuadPart-m_ltime.QuadPart;
   return (float) 1000 *diff / m_perfFreq;
}


LARGE_INTEGER CTimer::m_gtime;
int CTimer::m_dltime = 0;
BOOL CTimer::m_pause = FALSE;
LARGE_INTEGER CTimer::m_largeInt;
float CTimer::m_perfFreq = -1;

CTimer::CTimer(void)
{
	Reset();
}

CTimer::~CTimer(void)
{
}

void CTimer::UpdateClock()
{
	if (m_dltime>0) m_dltime--;
	if (m_pause) return;
	QueryPerformanceCounter(&m_gtime);
}

float CTimer::PeekTime()
{
  if (m_dltime>0) { Reset(); return 0; }
  return TimeDelta();
}

float CTimer::UpdatePeekTime()
{
  UpdateClock();
  return PeekTime();
}

float CTimer::GetTime()
{
  float my_time = PeekTime();
  Reset();
  return my_time;
}

float CTimer::UpdateGetTime()
{
   UpdateClock();
   return GetTime();
}

void CTimer::Reset()
{   
	if (m_perfFreq == -1) {
  	 if (!SUCCEEDED(QueryPerformanceFrequency(&m_largeInt))) {
	 	OutputDebugString("QueryPerformanceFrequency failed");
	 }
	 m_perfFreq = (float)m_largeInt.QuadPart;
	}
//	QueryPerformanceCounter(&m_gtime);
//	m_ltime.LowPart = m_gtime.LowPart;
//	m_ltime.HighPart = m_gtime.HighPart;
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

BOOL CTimer::CmpTime(float cmptm)
{
  BOOL rtval = (PeekTime() >= cmptm); 
  if (rtval) Reset(); 
  return rtval;
}

BOOL CTimer::CmpTime(void)
{
   return CmpTime(m_timeinterval);
}

void CTimer::setInterval(float interval)
{
	m_timeinterval = interval;
}

void CTimer::UnPause(void)
{
  OutputDebugString("Calling Un Pause\n");
	if (m_pause) m_dltime = 2;
	m_pause = FALSE;
}

BOOL CTimer::Paused(void)
{
	return m_pause;
}

BOOL CTimer::CmpTimeRaw(float cmptm)
{
	//FIX ME!!!
  float mytime = TimeDelta();
  float interval = TimeDelta();
 
  BOOL rtval = (interval >= cmptm); 
  //if (rtval) m_ltime = mytime;
  return rtval;
}

BOOL CTimer::CmpTimeRaw(void)
{
	return CmpTimeRaw(m_timeinterval);
}