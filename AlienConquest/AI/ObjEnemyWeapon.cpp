#include "StdAfx.h"
#include "objenemyweapon.h"
#include "objmgr.h"

int CObjEnemyWeapon::m_graph_init = 0;
CTexture *CObjEnemyWeapon::m_regular[1];

void CObjEnemyWeapon::paint()
{
	int anit = m_ani_time.GetTime();
	m_age -= anit;
	if (m_age < 0) g_ObjMgr->del(this);
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
	if (!m_graph_init) _LoadGraphics();
	m_max_x=800;
	m_age=1800;
	m_max_y=0;
	m_time.GetTime();
	m_ani_time.GetTime();
	m_z = 2;
}

CObjEnemyWeapon::~CObjEnemyWeapon(void)
{
	_UnloadGraphics();
}

void CObjEnemyWeapon::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,157,103,195,118);
   OutputDebugString("Loading Cenemy weapon graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
   m_graph_init = 1;
}

void CObjEnemyWeapon::_UnloadGraphics()
{
	delete m_regular[0];
}