#pragma once
#include "obj.h"
#include "Timer.h"
#include "Texture.h"
#include "Animate.h"
#include "Sound.h"

class CObjEnemy3 :
	public CObj
{
public:
	enum BEHAVIOUR { EXIT_STAGE_LEFT, RANDOM, STAYANDFIRE, EVADE };
	CObjEnemy3(void);
	virtual ~CObjEnemy3(void);
	void Fire();
	void Jet();
	void move();
	const static int m_numdamage=3;
	virtual void Collision(CObj *colwith);

private:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();

	void _Behaviour();
	BEHAVIOUR m_behaviour;
	CTimer m_behave_tim;

	static CTexture **m_regular;
	static CTexture **m_firing;
	static CTexture **m_jetting;
	static CTexture **m_hit;

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

	//static cSound *m_fire_sound;
};