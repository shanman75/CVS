#pragma once

#include "D3DObject.h"
#include "Timer.h"

class cCamera
{
public:
	enum EVENT { LEFT,RIGHT,UP,DOWN,FIRE, GOD, ZOOMIN,ZOOMOUT };
	cCamera(void);
	~cCamera(void);
	void SetCamera(void);
	void event(EVENT);
private:
	CTimer m_time;

	float m_x, m_y, m_z;
	float m_rotx, m_roty, m_zoom;
};
