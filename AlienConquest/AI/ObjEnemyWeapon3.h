#pragma once
#include "obj.h"
#include "Texture.h"

class CObjEnemyWeapon3 :
	public CObj
{
public:
	CObjEnemyWeapon3(void);
	virtual ~CObjEnemyWeapon3(void);
protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];
	//static CTexture *m_firing[4];

	static int m_graph_init;

	int m_fir_seq;
	CTimer m_ani_time;
	static RECT m_myboundrects[1];
};
