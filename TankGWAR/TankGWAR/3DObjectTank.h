#pragma once

#include "3DObject.h"
#include "3DObjectMissile.h"
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
  static enum SKINS { BLUE, BROWN, CAMOBLUE, GREY, GREEN, ORANGE, RAINBOW, RED, STEELBLUE };
  static const int NUM_SKINS = 9;

  c3DObjectTank();
	~c3DObjectTank(void);
	void paint(void);
//	void SetXYZ(float x, float y, float z) { m_xPos = x; m_yPos=y; m_zPos = z; }
  c3DObject * Fire(enum c3DObjectMissile::MSLTYPE tpe = c3DObjectMissile::MSLTYPE::SHELL);
  void event(enum EVENT, float amount=1.0f);
  void skin(enum SKINS sk) { m_skin = sk; }

  static const float tank_width;
  static const float tank_height;

protected:
  void MakeWorldMatrix(int x);
	void _LoadGraphics(void);
	void _UnloadGraphics();

  float m_turretRotate;
  float m_barrelHeight;
  float m_firePower;

  enum SKINS m_skin;

  CTimer m_keytime, m_firetime;
  
	static LPD3DXMESH			        m_tankmesh;
  static DWORD				          m_tankNmat;
  static LPDIRECT3DTEXTURE9*	  m_tanktex;
  static LPDIRECT3DTEXTURE9*    m_skintex;

  static D3DMATERIAL9*		      m_tankmat;
	static int					          m_graph_init;
};
