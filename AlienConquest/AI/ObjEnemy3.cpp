#include "StdAfx.h"
#include "objenemy3.h"
#include "ObjEnemyWeapon3.h"
#include "objmgr.h"

int CObjEnemy3::m_graph_init = 0;
CTexture *CObjEnemy3::m_regular[1];
CTexture *CObjEnemy3::m_firing[4];


CObjEnemy3::CObjEnemy3(void)
{
	CObj();
	if (!m_graph_init++) _LoadGraphics();
	m_max_x=5;
	m_max_y=2;
	m_time.GetTime();
	m_type = ENEMY;

	m_boundrectnum = 4;
	m_boundrects = new RECT [m_boundrectnum];
	SetRect((LPRECT)&m_boundrects[0],42,17,71,87);
	SetRect((LPRECT)&m_boundrects[1],76,35,114,52);
	SetRect((LPRECT)&m_boundrects[2],4,87,79,106);
	SetRect((LPRECT)&m_boundrects[3],79,85,109,96);

}

CObjEnemy3::~CObjEnemy3(void)
{
	if (!--m_graph_init)_UnloadGraphics();
}

void CObjEnemy3::paint()
{
	if (m_state == REGULAR && (m_fire_time.PeekTime() > 1500)) { m_fire_time.Reset(); Fire(); }

	switch (m_state) {
		case FIRING:
			m_curtexture = m_firing[m_fir_seq];
			if (m_fir_seq >= 3) m_state = REGULAR;
			if (m_ani_time.PeekTime() > 135) { m_fir_seq++; m_ani_time.Reset(); }
		    break;
		case REGULAR:
		default:
			m_curtexture = m_regular[0];
			break;
	};
	CObj::paint();
}

void CObjEnemy3::Fire()
{
	if (m_state != FIRING) {
		m_state = FIRING;
		m_fir_seq = 0;
		CObjEnemyWeapon3 *bull = new CObjEnemyWeapon3;
		bull->SetPosition(m_dpos_x+32,m_dpos_y+92);
		bull->SetSpeed(-250,0);
		bull->SetAccel(0,0);
		g_ObjMgr->add(bull);
	}
}

void CObjEnemy3::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,1,1,156,118);
   OutputDebugString("Loading Cenemy3 graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
   SetRect(&trect,1,119,156,236);
   for (int x=0; x < 4; x++) {
	   trect.left=x*156+1;
	   trect.right = trect.left +155;
	   m_firing[x]=new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
   }
   m_graph_init = 1;
}

void CObjEnemy3::_UnloadGraphics()
{
	delete m_regular[0];
	for (int x=0; x < 4; x++)
	  delete m_firing[x];
}