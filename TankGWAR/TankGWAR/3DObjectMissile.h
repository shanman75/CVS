#pragma once
#include "3dobject.h"

class c3DObjectMissile :
	public c3DObject
{
public:
	c3DObjectMissile(void);
	virtual ~c3DObjectMissile(void);
  void move();
protected:
	static LPD3DXMESH			m_missilemesh;
  static LPDIRECT3DTEXTURE9*	m_missiletex;
  static D3DMATERIAL9*		m_missilemat;
  static DWORD m_missileNmat;

	static int m_graph_init;

	void _LoadGraphics(void);
	void _UnloadGraphics();

  float m_initYvelocity;

};
