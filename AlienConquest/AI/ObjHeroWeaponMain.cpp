#include "StdAfx.h"
#include "objheroweaponmain.h"
//#include "objmgr.h"

int CObjHeroWeaponMain::m_graph_init = 0;
CTexture *CObjHeroWeaponMain::m_regular[3];

void CObjHeroWeaponMain::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[m_reg_seq%3];
			if (m_ani_tim.CmpTime(80)) m_reg_seq++;
			break;
	};   
	CObj::paint();
}

CObjHeroWeaponMain::CObjHeroWeaponMain(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=800;
	m_age=1800;
	m_max_y=0;
	m_z = 2;
	m_boundrectnum = 1;
	m_boundrects = new RECT [m_boundrectnum];
	SetRect((LPRECT)&m_boundrects[0],0,0,17,7);
	m_type = HEROWEAPON;
}

CObjHeroWeaponMain::~CObjHeroWeaponMain(void)
{
	if (!--m_graph_init) {_UnloadGraphics();}
	delete []m_boundrects;
}

void CObjHeroWeaponMain::_LoadGraphics()
{
   RECT trect;
//   SetRect(&trect,132,88,149,95);
   OutputDebugString("Loading Hero weapon graphics\n");
   SetRect(&trect,1,1,20,8);
   for (int x=0; x < 3; x++) {
	   trect.left=x*21+1;
	   trect.right = trect.left +20;
	   m_regular[x]=new CTexture("resource/bulletmap.bmp",0xFFFF00FF,&trect);
   }
}

void CObjHeroWeaponMain::_UnloadGraphics()
{
	delete m_regular[0];
}

void CObjHeroWeaponMain::Collision(CObj *colwith)
{
	CObj::Collision(colwith);
	g_ObjMgr->Score(100,m_playernum);
}
