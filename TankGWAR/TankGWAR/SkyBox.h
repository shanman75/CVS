#pragma once

#include "stdafx.h"
#include "D3DObject.h"

//#define SKY_HEIGHT 500
//#define SKY_WIDTH 2000
#define SKY_DEPTH 1000
#define SKY_HEIGHT 10
#define SKY_WIDTH 10

// DEFINES
#define		D3DFVF_SKYMESH		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
// STRUCTURES
struct sSkyVertex{	
	D3DXVECTOR3 pos;			// Position	
	D3DXVECTOR3 norm;			// Normals	
//	D3DCOLOR	diffuse;			// Colour	
	float	u, v;				// Texture Coordinates
};


struct BILLBOARDVERTEX
{ //billboard vertex
  D3DXVECTOR3 p;      //position
  float tu,tv; //texture coordinates
  static const DWORD FVF; //flexible vertex format
};



class cSkyBox
{
public:
  cSkyBox(void);
  ~cSkyBox(void);
  void Paint(void);
protected:
  BOOL _Init(void);
	static ID3DXMesh *m_SkyMesh;
	float *m_Heights;

  static LPDIRECT3DTEXTURE9*	  m_tertex;

  static LPDIRECT3DVERTEXBUFFER9 m_sbnegx;
  static LPDIRECT3DVERTEXBUFFER9 m_sbposx;
  static LPDIRECT3DVERTEXBUFFER9 m_sbnegz;
  static LPDIRECT3DVERTEXBUFFER9 m_sbposz;
  static LPDIRECT3DVERTEXBUFFER9 m_sbtop;


  float m_w,m_h; //width and height
};
