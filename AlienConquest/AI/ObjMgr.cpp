#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

CObjMgr *g_ObjMgr;
const int CObjMgr::m_numz=3;

CObjMgr::CObjMgr(void)
{
	m_numobj = 0;
	m_obj[0] = NULL;	
	m_spawn_interval = 1200;
}

CObjMgr::~CObjMgr(void)
{
	char buff[255];
	OutputDebugString("CObject Manager Dying\n");
	for (int x=0; x < m_numobj; x++) {
		sprintf (buff,"CObjectMgr - Killing %i\n",x);
		OutputDebugString(buff);
		SafeDelete(m_obj[x]);
	}
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
	newobj->SetPosition(800,300+rand()%200-100);
	newobj->SetSpeed(-150+rand()%50-25,rand()%50 - 25);
	add(newobj);
}

void CObjMgr::spawn(void)
{
	if (m_spawn_tim.PeekTime() > m_spawn_interval) {
		m_spawn_tim.Reset();
		// Choose a spawn type..only one now
		spawnOne();
	}
}

void CObjMgr::paint()
{
	for (int z=m_numz; z >= 0; z--) 
	  for (int x=0; x < m_numobj; x++)
		if (m_obj[x]->m_z == z) m_obj[x]->paint();	  
}

void CObjMgr::add(CObj *add)
{
	if (add!=NULL) m_obj[m_numobj++] = add;
}

void CObjMgr::del(CObj *del)
{
	if (del != NULL) {
		int found= 0;
		// Find the pointer
		for (int x = 0; x < m_numobj; x++)
		{
			if (!found && (m_obj[x] == del)) { found = 1; delete m_obj[x]; }
			if (found) { m_obj[x]= m_obj[x+1]; }
		}
		m_obj[m_numobj--] = NULL;
	}
	OutputDebugString("Deleted object\n");
}
