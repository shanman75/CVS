#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

CObjMgr *g_ObjMgr;
const int CObjMgr::m_numz=3;

CObjMgr::CObjMgr(void)
{
	m_numobj = 0;
	m_obj[0] = NULL;	
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
	for (int x=0; x < m_numobj; x++)
		m_obj[x]->move();
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
