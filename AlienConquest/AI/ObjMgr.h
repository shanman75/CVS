#pragma once

#include "Timer.h"
#include "Obj.h"
#include "ObjEnemy.h"
#include "ObjEnemy2.h"

class CObjMgr
{
public:
	CObjMgr(void);
	~CObjMgr(void);
	void move();
	void paint();
	void add(CObj *);
	void del(CObj *);
	static const int m_numz;

	const static int MAX_OBJECTS = 500;
	const static int MAX_ANIMATE = 10;
private:
	CObj *m_obj[MAX_OBJECTS];
	int m_numobj;
	CTimer m_time;
};

extern CObjMgr *g_ObjMgr;
