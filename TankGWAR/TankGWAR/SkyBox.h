#pragma once

#include "stdafx.h"
#include "D3DObject.h"

//#define SKY_HEIGHT 500
//#define SKY_WIDTH 2000
#define SKY_DEPTH 1000
#define SKY_HEIGHT 40
#define SKY_WIDTH 40

// DEFINES
#define		D3DFVF_SKYMESH		(D3DFVF_XYZ | D3DFVF_TEX1)
// STRUCTURES
struct sSkyVertex{	
	D3DXVECTOR3 pos;			// Position	
//	D3DCOLOR	diffuse;			// Colour	
	float	u, v;				// Texture Coordinates
};


struct BILLBOARDVERTEX
{ //billboard vertex
  D3DXVECTOR3 p;      //position
  float tu,tv; //texture coordinates
};


class cSkyBox
{
public:
  cSkyBox(void);
  ~cSkyBox(void);
  void Paint(void);

  void OnLostDevice(void);
  void OnResetDevice(void);
protected:
  BOOL _Init(void);
	static ID3DXMesh *m_SkyMesh;
	float *m_Heights;

  static LPDIRECT3DTEXTURE9*	  m_tertex;

  static LPDIRECT3DVERTEXBUFFER9 m_sbback;
  static LPDIRECT3DVERTEXBUFFER9 m_sbfront;
  static LPDIRECT3DVERTEXBUFFER9 m_sbleft;
  static LPDIRECT3DVERTEXBUFFER9 m_sbright;
  static LPDIRECT3DVERTEXBUFFER9 m_sbtop;


  static const DWORD m_FVF; //flexible vertex format
  float m_w,m_h; //width and height
};
