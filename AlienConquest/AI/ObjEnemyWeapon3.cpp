#include "StdAfx.h"
#include "objenemyweapon3.h"
//#include "objmgr.h"

int CObjEnemyWeapon3::m_graph_init = 0;
CTexture *CObjEnemyWeapon3::m_regular[1];

void CObjEnemyWeapon3::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[0];
			break;
	};   
	CObj::paint();
}

CObjEnemyWeapon3::CObjEnemyWeapon3(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=800;
	m_age=1800;
	m_max_y=0;
	m_ani_time.GetTime();
	m_z = 2;
	m_type = ENEMYWEAPON;

	m_boundrectnum = 1;
	m_boundrects = new RECT [m_boundrectnum];
	SetRect((LPRECT)&m_boundrects[0],1,1,35,12);
}

CObjEnemyWeapon3::~CObjEnemyWeapon3(void)
{
	if (!--m_graph_init) _UnloadGraphics();
	delete []m_boundrects;
}

void CObjEnemyWeapon3::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,157,103,195,118);
   OutputDebugString("Loading Cenemy weapon graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
}

void CObjEnemyWeapon3::_UnloadGraphics()
{
	delete m_regular[0];
}