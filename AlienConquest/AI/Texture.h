#pragma once
#include <d3d8.h>
#include <d3dx8.h>
#include "D3DObject.h"

//extern D3DObject *g_D3DObject;							// Main D3D Object

class CTexture
{
public:
	CTexture(char *filename, D3DCOLOR colorkey=0);
	~CTexture(void);
	Paint (RECT);
private:
	int m_height, m_width;
	IDirect3DTexture8 **m_textures;
};
