#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

CObjMgr *g_ObjMgr;
const int CObjMgr::m_numz=2;

CObjMgr::CObjMgr(void)
{
	m_numobj = 0;
	
}

CObjMgr::~CObjMgr(void)
{
	for (int x=0; x < m_numobj; x++)
		SafeDelete(m_obj[m_numobj]);
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
