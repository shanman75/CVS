#pragma once

#define TIMER_MAXDLTA 4294967294

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);
	const static DWORD maxtime = 65535;

	void UpdateClock();
	float GetTime();
	float UpdateGetTime();
	void  Reset();
	float PeekTime();
	float UpdatePeekTime();
	void TogglePause();
	void Pause();
	void UnPause();   // Forces unpause
	BOOL CmpTime(float cmptm);
	BOOL CmpTime(void);
	BOOL CmpTimeRaw(void);
	BOOL CmpTimeRaw(float cmptm);
	void setInterval(float);
	BOOL Paused(void);
	
private:
	LARGE_INTEGER m_ltime;
	static BOOL m_pause;
	static int m_dltime;
	float m_timeinterval;

	static LARGE_INTEGER m_gtime;
	static float m_perfFreq;
	static LARGE_INTEGER m_largeInt;

	float CTimer::GetHiTime(void);
	float TimeDelta(void);
};
