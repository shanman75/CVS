#pragma once
#include "3dobject.h"

class c3DObjectExplosion :
  public c3DObject
{
public:
  c3DObjectExplosion(void);
  ~c3DObjectExplosion(void);
  void paint(void);
private:
  CTimer m_exptime;

  static int m_graph_init;
	static LPD3DXMESH			        m_expmesh;
  static DWORD				          m_expNmat;
  static LPDIRECT3DTEXTURE9*	  m_exptex;
  static D3DMATERIAL9*		      m_expmat;

  void _LoadGraphics();
  void _UnloadGraphics();
};
