#pragma once

#include "stdafx.h"
#include "D3DObject.h"

#define DEF_TER_X 100
#define DEF_TER_Z 100
#define DEF_TER_WIDTH 5.0f

// DEFINES
//#define		D3DFVF_MESH		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define   D3DFVF_MESH   (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// STRUCTURES
struct sCustomVertex{	
	float		x, y, z;			// Position	
//	float   nx, ny, nz;			// Normals	
	D3DCOLOR	diffuse;			// Colour	
	float	u, v;				// Texture Coordinates
};


class cTerrain
{
public:
	enum EVENT { RAND };

	cTerrain(float ter_x=DEF_TER_X, float ter_z=DEF_TER_Z, float ter_width=DEF_TER_WIDTH);
	~cTerrain(void);
	void Paint(void);
	void RandomizeMesh(void);
	void event(EVENT evnt);
	float GetHeight(float x, float y);
  void SetHeight(float x, float z, float y);

private:
	void _Init(void);
	ID3DXMesh *g_TerrainMesh;
	ID3DXMesh *g_BigTerrainMesh;
	float *m_Heights;

  float TER_X, TER_Z, TER_WIDTH;

  static LPDIRECT3DTEXTURE9*	  m_tertex;
};
