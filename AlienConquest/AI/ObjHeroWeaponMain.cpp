#include "StdAfx.h"
#include "objHeroweaponmain.h"
//#include "objmgr.h"

int CObjHeroWeaponMain::m_graph_init = 0;
CTexture *CObjHeroWeaponMain::m_regular[3];
RECT CObjHeroWeaponMain::m_myboundrects[1];

void CObjHeroWeaponMain::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[m_reg_seq%3];
			if (m_ani_tim.CmpTime(90)) m_reg_seq++;
			break;
	};   
	CObj::paint();
}

CObjHeroWeaponMain::CObjHeroWeaponMain(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=800;
	m_age=1200;
	m_max_y=0;
	m_z = 2;
	m_boundrectnum = 1;
	m_boundrects = m_myboundrects;

	m_Hero = NULL;
	m_Hero2 = NULL;

	m_type = HEROWEAPON;
	m_reg_seq = 0;
}

CObjHeroWeaponMain::~CObjHeroWeaponMain(void)
{
	if (!--m_graph_init) {_UnloadGraphics();}
}

void CObjHeroWeaponMain::_LoadGraphics()
{
   RECT trect;
   OutputDebugString("Loading Hero weapon graphics\n");
   SetRect(&trect,185,1,196,6);
   for (int x=0; x < 3; x++) {
	   trect.left=x*12+185;
	   trect.right = trect.left +11;
	   m_regular[x]=new CTexture("resource/hero/heromap1.png",0xFFFF00FF,&trect);
   }
   	SetRect((LPRECT)&m_myboundrects[0],3,3,10,6);
}

void CObjHeroWeaponMain::_UnloadGraphics()
{
	for (int x = 0; x<3; x++)
	delete m_regular[x];
}

void CObjHeroWeaponMain::Collision(CObj *colwith)
{	
	if (m_Hero != NULL)
	m_Hero->Score(rand()%50+40);
	if (m_Hero2 != NULL)
	m_Hero2->Score(rand()%50+40);
	CObj::Collision(colwith);
}

void CObjHeroWeaponMain::SetHero(CHero *m)
{ 
	m_Hero = m;  
	m_Hero2 = NULL;  
}

void CObjHeroWeaponMain::SetHero(CHero2 *m)
{ 
	m_Hero = NULL;  
	m_Hero2 = m;  
}