#pragma once

#include "Timer.h"

typedef enum {
	REGULAR,FIRING,DYING,DEAD,JETTING
} EN_OBJ_STATE;

class CObj
{
friend class CObjMgr;
public:
	CObj(void);
	~CObj(void);
	virtual void paint();
	virtual void move();
	virtual void accel(float xdelta, float ydelta);
	void SetSpeed(float xspeed,float yspeed);
	void SetAccel(float xaccel,float yaccel);
	void SetPosition(float x, float y);

	const static float SSCALE;
	const static float ASCALE;
protected:
	float m_accel_x, m_accel_y;   // Acceleration X,Y
	float m_speed_x, m_speed_y;   // Current X,Y Speed
	int m_pos_x, m_pos_y;	   // Current X,Y Position
	float m_dpos_x, m_dpos_y;	// Current float x,y position
    float m_max_x, m_max_y;	   // Maximum X,Y Velocity
	CTimer m_time;				   // A Timer
	EN_OBJ_STATE m_state;
	int m_z;						// Defines the plane to be drawn on
};
