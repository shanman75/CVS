#include "StdAfx.h"
#include "objenemyweapon3.h"
#include "objmgr.h"

int CObjEnemyWeapon3::m_graph_init = 0;
CTexture *CObjEnemyWeapon3::m_regular[1];

void CObjEnemyWeapon3::paint()
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

CObjEnemyWeapon3::CObjEnemyWeapon3(void)
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

CObjEnemyWeapon3::~CObjEnemyWeapon3(void)
{
	_UnloadGraphics();
}

void CObjEnemyWeapon3::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,157,103,195,118);
   OutputDebugString("Loading Cenemy weapon graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
   m_graph_init = 1;
}

void CObjEnemyWeapon3::_UnloadGraphics()
{
	delete m_regular[0];
}