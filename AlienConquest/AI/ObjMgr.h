#pragma once

#include "Timer.h"
#include "World.h"
#include "Obj.h"
#include "Hero.h"
#include "BkGround.h"
#include "ObjEnemy.h"
#include "ObjEnemy2.h"
#include "ObjEnemy3.h"
#include "ObjHeroWeaponMain.h"
#include "ObjHeroWeaponMissile.h"

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
	void spawn(void);
	static const int m_numz;

	int GetNumObj(void) { return m_numobj; }

	const static int MAX_OBJECTS = 500;
	const static int MAX_ANIMATE = 10;
private:
	CObj *m_obj[MAX_OBJECTS];
	int m_numobj;
	CTimer m_time;
	CWorld m_world;
	CTimer m_spawn_tim;

	int m_spawn_interval;

	void spawnOne(void);
};

extern CObjMgr *g_ObjMgr;
