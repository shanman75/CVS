#pragma once
#include "obj.h"
#include "objmgr.h"
#include "Texture.h"


class CObjHeroWeaponMain :
	public CObj
{
public:
	CObjHeroWeaponMain(void);
	void SetHero(CHero *m);
	void SetHero(CHero2 *m);
	virtual ~CObjHeroWeaponMain(void);
	virtual void Collision(CObj *colwith);
protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[3];

	static int m_graph_init;

	int m_fir_seq;
	int m_reg_seq;

	CTimer m_ani_tim;

	static RECT m_myboundrects[1];

	CHero *m_Hero;
	CHero2 *m_Hero2;
};
