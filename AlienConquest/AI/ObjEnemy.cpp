#include "StdAfx.h"
#include "objenemy.h"

int CObjEnemy::m_graph_init = 0;
CTexture *CObjEnemy::m_regular[1];
CTexture *CObjEnemy::m_firing[4];


CObjEnemy::CObjEnemy(void)
{
	if (!m_graph_init) _LoadGraphics();
	m_pos_x = 200;
	m_pos_y = 200;
	m_max_x=5;
	m_max_y=2;
	m_time.GetTime();
}

CObjEnemy::~CObjEnemy(void)
{
	_UnloadGraphics();
}

void CObjEnemy::paint()
{
	D3DXVECTOR2 pnt;
	pnt.x = (int)m_pos_x;
	pnt.y = (int)m_pos_y;
	switch (m_state) {
		case FIRING:
			m_firing[m_fir_seq]->Paint(&pnt);
			if (m_fir_seq >= 3) m_state = REGULAR;
			if (m_ani_time.PeekTime() > 135) { m_fir_seq++; m_ani_time.Reset(); }
		    break;
		case REGULAR:
		default:
		    m_regular[0]->Paint(&pnt);
			break;
	};
}

void CObjEnemy::Fire()
{
	if (m_state != FIRING) {
		m_state = FIRING;
		m_fir_seq = 0;
	}
}

void CObjEnemy::_LoadGraphics()
{
   RECT trect;
   SetRect(&trect,1,1,156,118);
   OutputDebugString("Loading Cenemy graphics\n");
   m_regular[0] = new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
   SetRect(&trect,1,119,156,236);
   for (int x=0; x < 4; x++) {
	   trect.left=x*156+1;
	   trect.right = trect.left +155;
	   m_firing[x]=new CTexture("resource/enemyhead3.bmp",0xFFFF00FF,&trect);
   }
   m_graph_init = 1;
}

void CObjEnemy::_UnloadGraphics()
{
	delete m_regular[0];
	for (int x=0; x < 4; x++)
	  delete m_firing[x];
}