#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

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
	for (int x=0; x < m_numobj; x++)
		m_obj[x]->paint();
}

void CObjMgr::add(CObj *add)
{
	if (add!=NULL) m_obj[m_numobj++] = add;
}
