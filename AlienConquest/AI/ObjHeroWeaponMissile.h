#pragma once
#include "obj.h"
#include "objmgr.h"
#include "Texture.h"
#include "Animate.h"

class CObjHeroWeaponMissile :
	public CObj
{
public:
	CObjHeroWeaponMissile(void);
	virtual ~CObjHeroWeaponMissile(void);
	virtual void Collision(CObj *colwith);
	void SetHero(CHero *hero);
	void SetHero(CHero2 *hero2);

protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[4];

	static int m_graph_init;
	static RECT m_myboundrects[1];

	CAnimate m_fir_seq;

	CHero *m_Hero;
	CHero2 *m_Hero2;

	CTimer even_out;
};
