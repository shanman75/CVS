#pragma once
#include <d3d8.h>
#include <d3dx8.h>
#include "D3DObject.h"


//extern D3DObject *g_D3DObject;							// Main D3D Object

class CTexture
{
public:
	CTexture(char *filename, D3DCOLOR colorkey=0, RECT *srect = NULL, int maxw =128, int maxh = 128);
	~CTexture(void);
	void Paint (RECT *,D3DXVECTOR2 *);
	void Paint (D3DXVECTOR2 *);
	void Paint (int,int);
	void OnLostDevice();
	int GetWidth();
	int GetHeight();
private:
	void UpdateDeviceCaps(void);

	int m_height, m_width, m_numtex, m_nrow, m_ncol;
	IDirect3DTexture8 **m_textures;
	D3DXIMAGE_INFO m_texinfo;
	char *m_filename;
	int m_maxw;
	int m_maxh;
};
