#include "StdAfx.h"
#include "objheroweaponmissile.h"
//#include "objmgr.h"

int CObjHeroWeaponMissile::m_graph_init = 0;
CTexture *CObjHeroWeaponMissile::m_regular[1];

void CObjHeroWeaponMissile::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[0];
			break;
	};   
	CObj::paint();
}

CObjHeroWeaponMissile::CObjHeroWeaponMissile(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=450;
	m_age=2400;
	m_max_y=0;
	m_z = 2;
	m_boundrectnum = 1;
	m_boundrects = new RECT [m_boundrectnum];
	SetRect((LPRECT)&m_boundrects[0],0,0,18,8);
	m_type = HEROWEAPON;
}

CObjHeroWeaponMissile::~CObjHeroWeaponMissile(void)
{
	if (!--m_graph_init) {_UnloadGraphics();}
	delete []m_boundrects;
	OutputDebugString("HeroWeaponMissileDelete\n");
}

void CObjHeroWeaponMissile::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,132,88,149,95);
   OutputDebugString("Loading Hero weapon - missile graphics\n");
   m_regular[0] = new CTexture("resource/missledum.bmp",0xFFFF00FF,NULL);
}

void CObjHeroWeaponMissile::_UnloadGraphics()
{
	delete m_regular[0];
}