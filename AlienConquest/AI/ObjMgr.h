#pragma once

#include "Timer.h"
#include "Obj.h"

class CObjMgr
{
public:
	CObjMgr(void);
	~CObjMgr(void);
	void move();
	void paint();
	void add(CObj *);

	const static int MAX_OBJECTS = 500;
	const static int MAX_ANIMATE = 10;
private:
	CObj *m_obj[MAX_OBJECTS];
	int m_numobj;
	CTimer m_time;
};
