#pragma once
#include "obj.h"
#include "Timer.h"
#include "Texture.h"

class CObjEnemy3 :
	public CObj
{
public:
	CObjEnemy3(void);
	virtual ~CObjEnemy3(void);
	void Fire();
private:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];
	static CTexture *m_firing[4];

	static int m_graph_init;

	int m_fir_seq;
	CTimer m_ani_time;
	CTimer m_fire_time;
};
