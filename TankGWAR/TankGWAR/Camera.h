#pragma once

#include "D3DObject.h"
#include "Timer.h"

class cCamera
{
public:
	enum EVENT { LEFT,RIGHT,UP,DOWN,FIRE, GOD };
	cCamera(void);
	~cCamera(void);
	void SetCamera(void);
	void event(EVENT);
private:
	CTimer m_time;

	float m_x, m_y, m_z;
};
