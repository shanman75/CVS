#pragma once
#include "obj.h"
#include "Timer.h"
#include "Texture.h"

class CObjEnemy2 :
	public CObj
{
public:
	CObjEnemy2(void);
	~CObjEnemy2(void);
	void Fire();
	void Jet();
	void move();

private:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	
	static CTexture *m_regular[1];
	static CTexture *m_firing[3];
	static CTexture *m_jetting[4];

	static int m_graph_init;

	int m_fir_seq;
	int m_jet_seq;
	float m_jet_spd_x;
	float m_jet_accel_x;
	float m_org_max_x;
	CTimer m_ani_time;
};
