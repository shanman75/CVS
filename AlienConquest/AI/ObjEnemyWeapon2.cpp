#include "StdAfx.h"
#include "ObjEnemyWeapon2.h"
//#include "objmgr.h"

int CObjEnemyWeapon2::m_graph_init = 0;
CTexture *CObjEnemyWeapon2::m_regular[1];
RECT CObjEnemyWeapon2::m_myboundrects[1];

void CObjEnemyWeapon2::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
			m_curtexture = m_regular[0];
			break;
	};   
	CObj::paint();
}

CObjEnemyWeapon2::CObjEnemyWeapon2(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=600;
	m_age=700;
	m_max_y=0;
	m_ani_time.GetTime();
	m_z = 2;
	m_type = ENEMYWEAPON;

	m_boundrectnum = 1;
	m_boundrects = m_myboundrects;

}

CObjEnemyWeapon2::~CObjEnemyWeapon2(void)
{
	if (!--m_graph_init) _UnloadGraphics();
}

void CObjEnemyWeapon2::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,1,1,25,7);
   OutputDebugString("Loading Cenemy weapon graphics\n");
   m_regular[0] = new CTexture("resource/enemies/enemy2/bullet.png",0xFFFF00FF,&trect);
	
   SetRect((LPRECT)&m_myboundrects[0],1,1,21,5);
}

void CObjEnemyWeapon2::_UnloadGraphics()
{
	delete m_regular[0];
}