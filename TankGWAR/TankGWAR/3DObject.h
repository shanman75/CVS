#pragma once

#include "Timer.h"
#include "d3dx9.h"
//#include "3dobject.h"
#include "d3dobject.h"

class cGameState;

class c3DObject
{
friend class CObjMgr;
friend class cGameState;
public:
	c3DObject();
	virtual ~c3DObject(void);
	virtual void paint();
	virtual void move();

	void accel(D3DXVECTOR3);
  void pos(D3DXVECTOR3);
  void orient(D3DXVECTOR3);
  void velocity(D3DXVECTOR3);

protected:
  void c3DObject::MakeWorldMatrix( int x );

	CTimer m_time;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_orient;
	D3DXVECTOR3 m_velocity;
	D3DXVECTOR3 m_accel;

  D3DXVECTOR3 m_initVelocity;
  D3DXVECTOR3 m_initOrient;

	DWORD m_nMat;

	LPD3DXMESH m_curmesh;
  LPDIRECT3DTEXTURE9*	  m_curtex;
  D3DMATERIAL9*		      m_curmat;

};
