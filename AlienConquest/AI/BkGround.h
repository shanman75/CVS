#pragma once
#include "obj.h"
#include "Texture.h"
#include "Timer.h"

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
	static CTexture *m_regular[3];

	CTimer m_time_x;

	static int m_graph_init;
};
