#include "StdAfx.h"
#include "texture.h"

CTexture::CTexture(char *filename,D3DCOLOR colorkey)
{

	IDirect3DSurface8 *surf;
	g_D3DObject->LoadSurfaceFromFile(filename,&surf);

}

CTexture::~CTexture(void)
{
}

CTexture::Paint(RECT rect)
{

}
