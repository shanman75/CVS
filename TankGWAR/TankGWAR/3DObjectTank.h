#pragma once

#include "3DObject.h"
#include "D3DObject.h"
#include "Timer.h"


class cGameState;

class c3DObjectTank:
	public c3DObject
{
public:
  friend class cGameState;

  static enum FIRE_TYPE {MISSILE,ABOMB};
  static enum EVENT {UP,DOWN,LEFT,RIGHT,PWRUP,PWRDN};

  c3DObjectTank();
	~c3DObjectTank(void);
	void paint(void);
//	void SetXYZ(float x, float y, float z) { m_xPos = x; m_yPos=y; m_zPos = z; }
  c3DObject * Fire(enum FIRE_TYPE);
  void event(enum EVENT);

protected:
  void MakeWorldMatrix(int x);
	void _LoadGraphics(void);
	void _UnloadGraphics();

  float m_turretRotate;
  float m_barrelHeight;
  float m_firePower;

  CTimer m_keytime, m_firetime;
  
	static LPD3DXMESH			        m_tankmesh;
  static DWORD				          m_tankNmat;
  static LPDIRECT3DTEXTURE9*	  m_tanktex;
  static D3DMATERIAL9*		      m_tankmat;
	static int					          m_graph_init;
};
