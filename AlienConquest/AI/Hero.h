#pragma once
#include "obj.h"
#include "Texture.h"

class CHero :
	public CObj
{
public:
	CHero(void);
	~CHero(void);
private:
	virtual void paint();
	void _LoadGraphics(void);
	void _UnloadGraphics();

	static int m_graph_init;
	static CTexture *m_regular[1];
};
