#include "stdafx.h"
#include "minimap.h"

extern D3DObject *g_D3DObject;							// Main D3D Object

minimap::minimap()
{	map=NULL;   
	D3DXCreateSprite(g_D3DObject->m_d3ddevice9,&map);
}

void minimap::drawmap(float x,float y)
{	D3DXIMAGE_INFO structImageInfo; //image information
    LPDIRECT3DTEXTURE9 maptex=NULL;

    HRESULT hres=D3DXCreateTextureFromFileEx(g_D3DObject->m_d3ddevice9,
		"resource\\tankmap00-01.jpg",0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,D3DX_DEFAULT,0,&structImageInfo,NULL,&maptex);

	D3DXVECTOR3 mapcoordinates;
	mapcoordinates.x=x;
	mapcoordinates.y=y;
	mapcoordinates.z=0;

	map->Begin(NULL);
	map->Draw(maptex,NULL,NULL,&mapcoordinates,0xFFFFFFFF);
	map->End();

	maptex->Release();
}

minimap::~minimap()
{   map->Release();
}