#pragma once

class CTimer
{
public:
	CTimer(void);
	~CTimer(void);
	const static DWORD maxtime = 65535;
	void CTimer::UpdateClock();
	DWORD CTimer::GetTime();
	DWORD CTimer::UpdateGetTime();
	void CTimer::Reset();
	DWORD CTimer::PeekTime();
	DWORD CTimer::UpdatePeekTime();

private:
	DWORD m_ltime;
	static DWORD m_gtime;
};
