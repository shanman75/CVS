#pragma once
#include "obj.h"
#include "Texture.h"

class CHero :
	public CObj
{
public:
	CHero(void);
	virtual ~CHero(void);
	enum EVENT { LEFT,RIGHT,UP,DOWN,FIRE };
	enum POWERUP { PREG=0, PREG2=1, PMISSILE=2, PMISSILE2, PSPLIT, PLASER1, PLASER2 };
	void event(EVENT);
	void Fire();
	void FireWeapon(POWERUP);
	virtual void Collision(CObj *);
	static const int m_numweapons=10;
	int GetLives(void) { return m_lifes; }
private:
	virtual void paint();
	virtual void move();
	void _LoadGraphics(void);
	void _UnloadGraphics();

	int m_lifes;

	static int m_graph_init;
	
	static CTexture *m_regular[4];
	static CTexture *m_dying[1];

	CTimer m_mov_x, m_mov_y;
	CTimer m_fir_tim;
	CTimer m_ani_tim;
	CTimer m_dying_tim;


	int m_fir_seq;
	int m_reg_seq;

	CTimer m_powerup_tim[m_numweapons];
	BOOL m_powerup_eq[m_numweapons];
	DWORD m_powerup_rt[m_numweapons];
};
