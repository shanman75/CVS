#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DObject.h"


class CTexture
{
public:
	CTexture(char *filename, D3DCOLOR colorkey=0, RECT *srect = NULL, int maxw =128, int maxh = 128);
	~CTexture(void);
	void Paint (RECT *,D3DXVECTOR2 *);
	void Paint (D3DXVECTOR2 *);
	void Paint (float,float);
	void OnLostDevice();
	int GetWidth();
	int GetHeight();
private:
	void UpdateDeviceCaps(void);

	int m_height, m_width, m_numtex, m_nrow, m_ncol;
	IDirect3DTexture9 **m_textures;
	D3DXIMAGE_INFO m_texinfo;
	char *m_filename;
	int m_maxw;
	int m_maxh;
};
