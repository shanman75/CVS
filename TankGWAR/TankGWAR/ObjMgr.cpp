#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

CObjMgr g_ObjMgr;
const int CObjMgr::m_numz=3;

CObjMgr::CObjMgr(void)
{
	OutputDebugString("Object Manager Created\n");
	m_numobj=0;

	reset();
}

CObjMgr::~CObjMgr(void)
{
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

	int objdel = 0;
	for (int x=0; x < m_numobj; x++) {
		if (m_obj[x] && m_obj[x]->m_state == CObj::DEAD) { 
//			OutputDebugString("Dead object needs deleting\n");
			delete m_obj[x];
			m_obj[x] = NULL;
			objdel++;
		}
	}
	// Now compact the list (if objects where deleted)
	int bobj = 0;
	if (objdel) {
		for (int x=0; x < m_numobj; x++)
		{
			if (m_obj[x])
				m_obj[bobj++] = m_obj[x];
		}
		m_numobj -= objdel;
	}
}

void CObjMgr::move()
{
	// Move the world
	m_world.move();

	for (int x=0; x < m_numobj; x++)
		m_obj[x]->move();
}

void CObjMgr::paint()
{
	static char livestr[500];
	for (int z=m_numz; z >= 0; z--) 
	  for (int x=0; x < m_numobj; x++)
		if (m_obj[x] != NULL && m_obj[x]->m_z == z) m_obj[x]->paint();	

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

void CObjMgr::reset(void)
{
	if (m_numobj > 0)
	for (int x = 0; x < m_numobj ; x++)
		SafeDelete(m_obj[x]);
	m_numobj = 0;
	m_obj[0] = NULL;	

	m_world.reset();

}
