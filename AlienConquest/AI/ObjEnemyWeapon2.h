#pragma once
#include "obj.h"
#include "Texture.h"

class CObjEnemyWeapon2 :
	public CObj
{
public:
	CObjEnemyWeapon2(void);
	virtual ~CObjEnemyWeapon2(void);
protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];
	//static CTexture *m_firing[4];

	static int m_graph_init;

	int m_fir_seq;
	CTimer m_ani_time;
};
