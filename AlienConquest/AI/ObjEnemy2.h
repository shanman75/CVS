#pragma once

#include "obj.h"
#include "Timer.h"
#include "Texture.h"
#include "Animate.h"

class CObjEnemy2 :
	public CObj
{
public:
	enum BEHAVIOUR { EXIT_STAGE_LEFT, RANDOM, STAYANDFIRE, EVADE };
	CObjEnemy2(void);
	virtual ~CObjEnemy2(void);
	void Fire();
	void Jet();
	void move();
	virtual void Collision(CObj *);

	static const int m_numdamage = 3;

private:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	void _Behaviour();

	BEHAVIOUR m_behaviour;
	CTimer m_behave_tim;
	
	static CTexture **m_hit;  // 2
	static CTexture **m_regular;  // 3*numdamage
	static CTexture **m_firing;   // 3*num damage
	static CTexture **m_jetting;  // 4* num damage

	static int m_graph_init;

	// Animation Variables
	CAnimate m_jet_seq, m_fir_seq, m_hit_seq;

	// Timers
	CTimer m_ani_time, m_jet_time, m_fire_time, m_age_time, m_rand_time;
	int m_jet_nxt, m_fir_nxt;

	float m_jet_spd_x;
	float m_jet_accel_x;
	float m_org_max_x;

	int m_curdamage;

	static RECT m_myboundrects[4];
	static const char *m_grdir;
};
