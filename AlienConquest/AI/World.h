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
	float CullX(float in, float width) {
							if (in < m_cur_x) return m_cur_x;
							else if (in+width > m_cur_x+m_scr_width) return m_cur_x+m_scr_width-width;
							else return in;
						   }
	float CullY(float in, float height) {
							if (in < m_cur_y) return m_cur_y;
							else if (in+height > m_cur_y+m_scr_height) return m_cur_y+m_scr_height-height;
							else return in;
						   }
	BOOL IsWayLeft(D3DXVECTOR2 *p, D3DXVECTOR2 *t);
	float GetCurXEdge(void) { return m_cur_x+m_scr_width; }

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
