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
private:
	virtual void paint();
	virtual void move();
	void _LoadGraphics(void);
	void _UnloadGraphics();

	static int m_graph_init;
	static CTexture *m_regular[1];
	CTimer m_mov_x, m_mov_y;
};
