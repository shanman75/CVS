#pragma once

#include "stdafx.h"
#include "D3DObject.h"

#define TER_X 200
#define TER_Y 200
#define TER_WIDTH 5.0f

// DEFINES
#define		D3DFVF_MESH		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
// STRUCTURES
struct sCustomVertex{	
	float		x, y, z;			// Position	
	float   	nx, ny, nz;			// Normals	
//	D3DCOLOR	diffuse;			// Colour	
	float	u, v;				// Texture Coordinates
};


class cTerrain
{
public:
	enum EVENT { RAND };

	cTerrain(void);
	~cTerrain(void);
	void Paint(void);
	void RandomizeMesh(void);
	void event(EVENT evnt);
	float GetHeight(int x, int y) { return m_Heights[x* (TER_Y + 1) + y]; }
private:
	void _Init(void);
	ID3DXMesh *g_TerrainMesh;
	float *m_Heights;

  static LPDIRECT3DTEXTURE9*	  m_tertex;
};
