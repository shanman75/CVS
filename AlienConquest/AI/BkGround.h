#pragma once
#include "obj.h"
#include "Texture.h"
#include "Timer.h"
#include "World.h"

class CBkGround :
	public CObj
{
public:
	CBkGround(void);
	virtual ~CBkGround(void);
private:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_top[3];
	static CTexture *m_middle[3];
	static CTexture *m_bottom[3];

	CTimer m_time_x;

	static int m_graph_init;

	CWorld m_world;
};
