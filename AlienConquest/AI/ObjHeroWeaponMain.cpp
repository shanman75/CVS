#include "StdAfx.h"
#include "objheroweaponmain.h"
//#include "objmgr.h"

int CObjHeroWeaponMain::m_graph_init = 0;
CTexture *CObjHeroWeaponMain::m_regular[1];

void CObjHeroWeaponMain::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[0];
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
	SetRect((LPRECT)&m_boundrects[0],0,0,18,8);
	m_type = HEROWEAPON;
}

CObjHeroWeaponMain::~CObjHeroWeaponMain(void)
{
	if (!--m_graph_init) {_UnloadGraphics();}
	//if (m_boundrects) delete []m_boundrects;
}

void CObjHeroWeaponMain::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,132,88,149,95);
   OutputDebugString("Loading Hero weapon graphics\n");
   m_regular[0] = new CTexture("resource/machingundum.bmp",0xFFFF00FF,NULL);
   m_graph_init = 1;
}

void CObjHeroWeaponMain::_UnloadGraphics()
{
	delete m_regular[0];
}