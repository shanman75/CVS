#pragma once

#include "stdafx.h"
#include "Timer.h"
#include "World.h"
#include "Obj.h"
#include "3DObject.h"
#include "3DObjectTank.h"

class CObjMgr
{
public:
	CObjMgr(void);
	~CObjMgr(void);
	void move();
	void paint();

	void coldet();

	void add(CObj *);
	void del(CObj *);
	void add(c3DObject *);
	void del(c3DObject *);

	void reset(void);

	static const int m_numz;

	int GetNumObj(void) { return m_numobj; }

	const static int MAX_OBJECTS = 700;
	const static int MAX_3DOBJECTS = 255;
	const static int MAX_ANIMATE = 20;
private:
	CObj       *m_obj[MAX_OBJECTS];
	c3DObject  *m_3Dobj[MAX_3DOBJECTS];
	int m_numobj, m_num3Dobj;
	CTimer m_time;
	CWorld m_world;
	CTimer m_spawn_tim;
//  cCamera *m_curcamera;  
};

extern CObjMgr *g_ObjMgr;
