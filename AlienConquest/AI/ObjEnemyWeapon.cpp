#include "StdAfx.h"
#include "objenemyweapon.h"
//#include "objmgr.h"

int CObjEnemyWeapon::m_graph_init = 0;
CTexture *CObjEnemyWeapon::m_regular[1];

void CObjEnemyWeapon::paint()
{
	switch (m_state) {
		case REGULAR:
		default:
		    m_curtexture = m_regular[0];
			break;
	};   
	CObj::paint();
}

CObjEnemyWeapon::CObjEnemyWeapon(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=800;
	m_age=800;
	m_max_y=0;
	m_z = 2;
	m_boundrectnum = 1;
	m_boundrects = new RECT [m_boundrectnum];
	SetRect((LPRECT)&m_boundrects[0],0,0,18,8);
	m_type = ENEMYWEAPON;
}

CObjEnemyWeapon::~CObjEnemyWeapon(void)
{
	if (!--m_graph_init) {_UnloadGraphics();}
	//if (m_boundrects) delete []m_boundrects;
}

void CObjEnemyWeapon::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,132,88,149,95);
   OutputDebugString("Loading Cenemy weapon graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead1.bmp",0xFFFF00FF,&trect);
   m_graph_init = 1;
}

void CObjEnemyWeapon::_UnloadGraphics()
{
	delete m_regular[0];
}