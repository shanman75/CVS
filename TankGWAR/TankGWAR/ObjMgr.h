#pragma once

#include "Timer.h"
#include "World.h"
#include "Obj.h"
/*
#include "Text.h"
#include "Hero.h"
#include "Hero2.h"
#include "BkGround.h"
#include "ObjEnemy.h"
#include "ObjEnemy2.h"
#include "ObjEnemy3.h"
#include "ObjEnemyWeapon.h"
#include "ObjEnemyWeapon2.h"
#include "ObjEnemyWeapon3.h"
#include "ObjHeroWeaponMain.h"
#include "ObjHeroWeaponMissile.h"
#include "ObjHeroWeaponLaser.h"
#include "ObjBmp.h"
#include "ObjPowerUp.h"
*/

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

	void reset(void);

	static const int m_numz;

	int GetNumObj(void) { return m_numobj; }

	const static int MAX_OBJECTS = 700;
	const static int MAX_ANIMATE = 20;
private:
	CObj *m_obj[MAX_OBJECTS];
	int m_numobj;
	CTimer m_time;
	CWorld m_world;
	CTimer m_spawn_tim;

};

extern CObjMgr g_ObjMgr;
