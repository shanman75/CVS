#pragma once
#include "3dobject.h"

class cGameState;

class c3DObjectExplosion :
  public c3DObject
{
friend class cGameState;
public:
  c3DObjectExplosion(float radius = 50.0f,float exp_dur = 6000.0f, 
    D3DCOLORVALUE col = D3DXCOLOR(1.0f,0.0f,0.1f,0.8f));
  ~c3DObjectExplosion(void);
  void paint(void);
protected:
  float m_radius;
private:
  CTimer m_exptime;

  static int m_graph_init;
	static LPD3DXMESH			        m_expmesh;
  static DWORD				          m_expNmat;
  static LPDIRECT3DTEXTURE9*	  m_exptex;
  static D3DMATERIAL9*		      m_expmat;

  float m_expdur;
  D3DCOLORVALUE m_expcolor;

  void _LoadGraphics();
  void _UnloadGraphics();
};
