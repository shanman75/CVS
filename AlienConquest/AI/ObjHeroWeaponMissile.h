#pragma once
#include "obj.h"
#include "Texture.h"

class CObjHeroWeaponMissile :
	public CObj
{
public:
	CObjHeroWeaponMissile(void);
	virtual ~CObjHeroWeaponMissile(void);
protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];

	static int m_graph_init;

	int m_fir_seq;
	CTimer m_ani_time;
};
