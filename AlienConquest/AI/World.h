#pragma once

#include "Timer.h"
#include "d3dx8.h"

class CWorld
{
public:
	CWorld(void);
	~CWorld(void);
	void move (void);
	BOOL ToScreen (D3DXVECTOR2 *pnt, D3DXVECTOR2 *tex);

private:

	void _Init(void);

	static int m_width;
	static int m_height;
	
	static int m_scr_height;
	static int m_scr_width;

	static int m_speed_x;
	static int m_speed_y;

	static int m_init;

	static float m_cur_x;
	static float m_cur_y;

	CTimer m_timer;
};
