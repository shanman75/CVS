#include "stdafx.h"
#include "minimap.h"

extern D3DObject *g_D3DObject;							// Main D3D Object
extern LPD3DXSPRITE map;
extern LPDIRECT3DTEXTURE9 maptex;

void drawmap()
{	D3DXIMAGE_INFO structImageInfo; //image information
    
    HRESULT hres=D3DXCreateTextureFromFileEx(g_D3DObject->m_d3ddevice9,
		"resource\\tankmap00-01.jpg",0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,D3DX_DEFAULT,0,&structImageInfo,NULL,&maptex);

	D3DXVECTOR3 mapcoordinates;
//	mapcoordinates.x=#;
//	mapcoordinates.y=#;
//	mapcoordinates.z=0;


	map->Begin(NULL);
	map->Draw(maptex,NULL,NULL,NULL,0xFFFFFFFF);
	map->End();

	maptex->Release();

}