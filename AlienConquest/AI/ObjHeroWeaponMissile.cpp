#include "StdAfx.h"
#include "objheroweaponmissile.h"
//#include "objmgr.h"

int CObjHeroWeaponMissile::m_graph_init = 0;
CTexture *CObjHeroWeaponMissile::m_regular[4];
RECT CObjHeroWeaponMissile::m_myboundrects[1];

void CObjHeroWeaponMissile::paint()
{
	if (even_out.CmpTime(400)) { m_accel_y = 0; m_speed_y = 0; m_accel_x+=400;}
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[m_fir_seq.GetCurFrame()];
			break;
	};   
	CObj::paint();
}

CObjHeroWeaponMissile::CObjHeroWeaponMissile(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=950;
	m_age=1600;
	m_max_y=1000;
	m_z = 2;
	m_boundrectnum = 1;
	m_boundrects = m_myboundrects;
	m_type = HEROWEAPON;
	even_out.Reset();

	m_Hero = NULL;
	m_Hero2 = NULL;

	m_fir_seq.SetParams(4,CAnimate::OSCILLATE,80,3200,0);
}

CObjHeroWeaponMissile::~CObjHeroWeaponMissile(void)
{
	OutputDebugString("HeroWeaponMissileDelete\n");
	if (!--m_graph_init) {_UnloadGraphics();}
}

void CObjHeroWeaponMissile::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,132,88,149,95);
   OutputDebugString("Loading Hero weapon - missile graphics\n");
   SetRect(&trect,185,13,203,20);
   for (int x=0; x < 4; x++) {
	   trect.left=x*19+185;
	   trect.right = trect.left +18;
	   m_regular[x]=new CTexture("resource/hero/heromap1.png",0xFFFF00FF,&trect);
   }

   SetRect((LPRECT)&m_myboundrects[0],7,4,8,13);
}

void CObjHeroWeaponMissile::_UnloadGraphics()
{
	for (int x=0; x<4; x++)
	delete m_regular[x];
}

void CObjHeroWeaponMissile::Collision(CObj *colwith)
{
	if (m_Hero != NULL)
	m_Hero->Score(rand()%100+80);
	if (m_Hero2 != NULL)
	m_Hero2->Score(rand()%100+80);
	CObj::Collision(colwith);
}

void CObjHeroWeaponMissile::SetHero(CHero *m)
{ 
	m_Hero = m;  
	m_Hero2 = NULL;  
}
void CObjHeroWeaponMissile::SetHero(CHero2 *m)
{ 
	m_Hero = NULL;  
	m_Hero2 = m;  
}
