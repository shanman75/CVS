#pragma once

#include "Timer.h"
#include "Texture.h"
#include "World.h"

class CObj
{
friend class CObjMgr;
public:
	CObj(void);
	virtual ~CObj(void);
	virtual void paint();
	virtual void move();
	virtual void accel(float xdelta, float ydelta);
	void SetSpeed(float xspeed,float yspeed);
	void SetAccel(float xaccel,float yaccel);
	void SetPosition(float x, float y);
	BOOL CollisionDet(CObj *colwith);
	virtual void Collision(CObj *colwith);
	static BOOL TestRect(const RECT *x, const POINT, const RECT *y, const POINT);
	virtual BOOL done(void);

	enum STATE { REGULAR, FIRING, DYING, DEAD, JETTING, GHOST, HIT };
	enum OTYPE { NOTHING, HERO, HEROWEAPON, HEROINVINCIBLE, HERODEAD, ENEMY, ENEMYWEAPON, O_POWERUP };

	OTYPE GetType(void) { return m_type;}

	BOOL CanCollide(OTYPE, OTYPE);

	const static float SSCALE;
	const static float ASCALE;
protected:
	float m_accel_x, m_accel_y;		// Acceleration X,Y
	float m_speed_x, m_speed_y;		// Current X,Y Speed
	int m_pos_x, m_pos_y;			// Current X,Y Position
	float m_dpos_x, m_dpos_y;		// Current float x,y position
    float m_max_x, m_max_y;			// Maximum X,Y Velocity
	CTimer m_time;					// A Timer
	STATE m_state;
	int m_z;						// Defines the plane to be drawn on
	CTexture *m_curtexture;			// Current frame to paint
	RECT *m_boundrects;				// Array of bounding rectangles
	int m_boundrectnum;				// Boundrect number
	long m_age;						// Age of object (-1 = perm, any other value is ms)
	CTimer m_age_timer;				// Age timer
    OTYPE m_type;
	CWorld m_world;
	BOOL m_stayonscr;				// Stay on the screen?
	int m_playernum;
	int m_done;

	static LPRECT m_tr_x, m_tr_y;
};
