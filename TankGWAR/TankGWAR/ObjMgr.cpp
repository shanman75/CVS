#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

CObjMgr *g_ObjMgr;
const int CObjMgr::m_numz=3;


CObjMgr::CObjMgr(void)
{
	OutputDebugString("Object Manager Created\n");
	m_numobj    =	0;
	m_num3Dobj  =	0;

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
  for (int z=0; z < m_num3Dobj; z++)
		m_3Dobj[z]->move();
  for (int z=0; z < m_num3Dobj; z++)
    if(m_3Dobj[z]->m_position.y < 0)
		   del(m_3Dobj[z]);
}

void CObjMgr::paint()
{
	//static char livestr[500];
  for (int y=0; y < m_num3Dobj; y++)
    if (m_3Dobj[y] != NULL) m_3Dobj[y]->paint();

  if (m_numz > 0) {
    g_D3DObject->m_pd3dxSprite->Begin(0);
	  for (int z=m_numz; z >= 0; z--) 
	    for (int x=0; x < m_numobj; x++)
		    if (m_obj[x] != NULL && m_obj[x]->m_z == z) m_obj[x]->paint();	
    g_D3DObject->m_pd3dxSprite->End();
  }

}

void CObjMgr::add(CObj *add)
{
	if (add!=NULL) m_obj[m_numobj++] = add;
}

void CObjMgr::add(c3DObject *add)
{
	if (add!=NULL) m_3Dobj[m_num3Dobj++] = add;
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

void CObjMgr::del(c3DObject *del)
{
	OutputDebugString("Deleting 3D object\n");
	if (del != NULL) {
		int found= 0;
		// Find the pointer
		for (int x = 0; x < m_num3Dobj; x++)
		{
			if (m_3Dobj[x] == del) { found = 1; delete m_3Dobj[x]; }
			if (found) { m_3Dobj[x]= m_3Dobj[x+1]; }
		}
		if (found)
		 m_3Dobj[m_num3Dobj--] = NULL;
		else
		 OutputDebugString("3D Object NOT found\n");
	}
	OutputDebugString("Deleted 3D object\n");
}

void CObjMgr::reset(void)
{
	if (m_numobj > 0)
	for (int x = 0; x < m_numobj ; x++)
		SafeDelete(m_obj[x]);
  if (m_num3Dobj > 0)
	for (int y = 0; y < m_num3Dobj ; y++)
		SafeDelete(m_3Dobj[y]);

	m_numobj = 0;
	m_obj[0] = NULL;	
	m_num3Dobj = 0;
	m_3Dobj[0] = NULL;	

	m_world.reset();

}
