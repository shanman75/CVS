#pragma once

#include "Timer.h"

typedef enum {
	REGULAR,FIRING,DYING,DEAD,JETTING
} EN_OBJ_STATE;

class CObj
{
public:
	CObj(void);
	~CObj(void);
	virtual void paint();
	virtual void move();
	void SetSpeed(double xspeed,double yspeed);
	void SetAccel(double xaccel,double yaccel);
	void SetPosition(double x, double y);
	const static double SSCALE;
	const static double ASCALE;
protected:
	double m_accel_x, m_accel_y;   // Acceleration X,Y
	double m_speed_x, m_speed_y;   // Current X,Y Speed
	int m_pos_x, m_pos_y;	   // Current X,Y Position
	double m_dpos_x, m_dpos_y;	// Current Double x,y position
    double m_max_x, m_max_y;	   // Maximum X,Y Velocity
	CTimer m_time;				   // A Timer
	EN_OBJ_STATE m_state;
};
