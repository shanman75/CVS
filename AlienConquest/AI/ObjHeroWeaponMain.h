#pragma once
#include "obj.h"
#include "objmgr.h"
#include "Texture.h"

class CObjHeroWeaponMain :
	public CObj
{
public:
	CObjHeroWeaponMain(void);
	virtual ~CObjHeroWeaponMain(void);
	virtual void Collision(CObj *colwith);
protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];

	static int m_graph_init;

	int m_fir_seq;
	CTimer m_ani_time;
};
