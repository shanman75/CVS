#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

CObjMgr *g_ObjMgr;
const int CObjMgr::m_numz=3;
char CObjMgr::m_scoreline[500];

CObjMgr::CObjMgr(void)
{
	m_numobj=0;
	reset();
}

CObjMgr::~CObjMgr(void)
{
	//char buff[255];
	//OutputDebugString("CObject Manager Dying\n");
	reset();
}

void CObjMgr::coldet(void)
{
	for (int x=0; x < m_numobj; x++)
		for (int y=x+1; y < m_numobj; y++)
			if (m_obj[x]->CollisionDet(m_obj[y])) {

				m_obj[x]->Collision(m_obj[y]);
				m_obj[y]->Collision(m_obj[x]);

			};

	for (int x=m_numobj-1; x >= 0; x--) {
		if (m_obj[x]->m_state == CObj::DEAD) { 
			OutputDebugString("Dead object needs deleting\n");
			del(m_obj[x]); 
		}
	}
}

void CObjMgr::move()
{
	// Move the world
	m_world.move();

	for (int x=0; x < m_numobj; x++)
		m_obj[x]->move();
}

void CObjMgr::spawnOne(void)
{
	int objtype = rand() % 3;
	CObj *newobj;

	// Choose an object type
	switch (objtype)
	{
	case 0:
		newobj = new CObjEnemy;
		break;
	case 1:
		newobj = new CObjEnemy2;
		break;
	default:
	case 2:
		newobj = new CObjEnemy3;
		break;
	}
	newobj->SetPosition((float)(m_world.GetCurXEdge()+10),(float)(300+rand()%290-145));
	newobj->SetSpeed((float)(-150+rand()%50-25),(float)(rand()%50 - 25));
	add(newobj);
}

void CObjMgr::spawn(void)
{
	if (m_spawn_tim.CmpTime(m_spawn_interval)) {
		// Choose a spawn type..only one now
		int spn = rand()%50;
		if (spn < 35)
			spawnOne();
		else if (spn < 45)
		{
			spawnOne();
			spawnOne();
		}
		else {
			spawnOne(); spawnOne(); spawnOne();
		}
		// Choose next spawn interval

		m_spawn_interval = 500+rand()%500;
	}
}

void CObjMgr::paint()
{
	for (int z=m_numz; z >= 0; z--) 
	  for (int x=0; x < m_numobj; x++)
		if (m_obj[x]->m_z == z) m_obj[x]->paint();	

	// Paint DashBoard
	//sprintf(m_scoreline,"%i",m_player1_score);
	//g_D3DObject->DrawTextStr(5,5,0xFFFF00FF,m_scoreline);
	m_D3DText.DrawScore(m_player1_score,m_player1_scorexy);
}

void CObjMgr::add(CObj *add)
{
	if (add!=NULL) m_obj[m_numobj++] = add;
}

void CObjMgr::del(CObj *del)
{
//	OutputDebugString("Deleting object\n");
	if (del != NULL) {
		int found= 0;
		// Find the pointer
		for (int x = 0; x < m_numobj; x++)
		{
			if (m_obj[x] == del) { found = 1; delete m_obj[x]; }
			if (found) { m_obj[x]= m_obj[x+1]; }
		}
		if (found)
		 m_obj[m_numobj--] = NULL;
		else
		 OutputDebugString("Object NOT found\n");
	}
	OutputDebugString("Deleted object\n");
}

void CObjMgr::Score(int score, int playernum)
{
	if (playernum == 1) { this->m_player1_score += score; }
}

void CObjMgr::reset(void)
{
	if (m_numobj > 0)
	for (int x = 0; x < m_numobj ; x++)
		SafeDelete(m_obj[x]);
	m_numobj = 0;
	m_obj[0] = NULL;	
	m_spawn_interval = 1200;
	m_player1_score = 0;
	m_player1_scorexy.x =150;
	m_player1_scorexy.y = 5;
}