#pragma once
#include "obj.h"
#include "Texture.h"

class CHero :
	public CObj
{
public:
	CHero(void);
	~CHero(void);
	enum EVENT { LEFT,RIGHT,UP,DOWN,FIRE };
	void event(EVENT);
	void Fire();
	virtual void Collision(CObj *);
private:
	virtual void paint();
	virtual void move();
	void _LoadGraphics(void);
	void _UnloadGraphics();

	static int m_graph_init;
	
	static CTexture *m_regular[1];
	static CTexture *m_dying[1];

	CTimer m_mov_x, m_mov_y;
	CTimer m_fir_tim;
	CTimer m_ani_tim;

	int m_fir_seq;
};
