#pragma once

#define TIMER_MAXDLTA 4294967294

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);
	const static DWORD maxtime = 65535;
	void UpdateClock();
	DWORD GetTime();
	DWORD UpdateGetTime();
	void  Reset();
	DWORD PeekTime();
	DWORD UpdatePeekTime();
	void TogglePause();
	void Pause();
	void UnPause();   // Forces unpause
	BOOL CmpTime(DWORD cmptm);
	BOOL CmpTime(void);
	BOOL CmpTimeRaw(void);
	BOOL CmpTimeRaw(DWORD cmptm);
	void setInterval(DWORD);
	BOOL Paused(void);
	
private:
	DWORD m_ltime;
	static DWORD m_gtime;
	static BOOL m_pause;
	static int m_dltime;
	int m_timeinterval;
};
