#include "StdAfx.h"
#include "terrain.h"
#include <d3dx9.h>
#include <stdio.h>

LPDIRECT3DTEXTURE9*	  cTerrain::m_tertex;

cTerrain::cTerrain(void)
{
	_Init();
}

void cTerrain::Paint()
{
	D3DXMATRIXA16 matwrld;
  D3DMATERIAL9 mtrl;
  ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	D3DXMatrixIdentity(&matwrld);
	g_D3DObject->m_d3ddevice9->SetTransform(D3DTS_WORLD,&matwrld);
  mtrl.Diffuse.r = 1.0f;
  mtrl.Diffuse.g = 1.0f;
  mtrl.Diffuse.b = 1.0f;
  mtrl.Diffuse.a = 1.0f;
//  mtrl.Specular = mtrl.Diffuse;
//  mtrl.Power = 100.0f;
  g_D3DObject->m_d3ddevice9->SetMaterial( &mtrl );

  if (g_TerrainMesh != NULL)
  {   
    g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
	  g_TerrainMesh->DrawSubset(0);
  }
}

void cTerrain::_Init()
{

  D3DXCreateMeshFVF( 2*TER_X*TER_Y, (TER_X + 1) * (TER_Y + 1) , 
	  D3DXMESH_32BIT | D3DXMESH_SYSTEMMEM, 
	  D3DFVF_MESH,
	  g_D3DObject->m_d3ddevice9, 
	  &g_TerrainMesh 
	);

	m_Heights = new float[(TER_X + 1) * (TER_Y + 1)];
  for (int x = 0; x < (TER_X+1)*(TER_Y+1); x++)
    m_Heights[x] = 1.0f;
  
	LPDIRECT3DVERTEXBUFFER9 pTetVB = NULL; //Pointer to vertex buffer.

  char texpath[1024];
  LPDIRECT3DTEXTURE9 tempt;

  m_tertex = new LPDIRECT3DTEXTURE9 [1];
  sprintf (texpath,"resource\\%s","texture3.bmp");
  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
    return;
  m_tertex[0] = tempt;

	RandomizeMesh();
}

D3DCOLOR randcolor()
{
	switch (rand()%3) {
		case 3:
			return D3DCOLOR_RGBA(180,141,3,255); break;
		case 2:
			return D3DCOLOR_RGBA(119,150,108,255); break;
		case 1:
			return D3DCOLOR_RGBA(89,83,60,255); break;
		default:
			return D3DCOLOR_RGBA(90,75,10,255); break;
	}
}

void cTerrain::event(EVENT evnt)
{
	  switch (evnt) {
		  case RAND: RandomizeMesh(); break;
		  default: break;
	  }
}
void cTerrain::RandomizeMesh(void)
{
	sCustomVertex *VertexPtr;
	DWORD *IndexPtr;
	float *t_Heights = m_Heights;

	IndexPtr =NULL;
	VertexPtr =NULL;

	g_TerrainMesh->LockVertexBuffer( 0, (LPVOID *) &VertexPtr );	
	g_TerrainMesh->LockIndexBuffer( 0, (LPVOID *) &IndexPtr );	

    for (int i = 0; i < (TER_X + 1); i++)
	  for (int j = 0; j < (TER_Y + 1); j++) {
		  VertexPtr->x = (i - (TER_X-1)/2) * TER_WIDTH;
		  VertexPtr->z = (j - (TER_Y-1)/2) * TER_WIDTH;
		  VertexPtr->y = (float) (rand()%705/5);
      //VertexPtr->y = -5;
		  VertexPtr->nx = 0.0f;
		  VertexPtr->ny = 1.0f;
		  VertexPtr->nz = 0.0f;
//		  VertexPtr->diffuse = randcolor();
//		  VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,0,255);
 		  VertexPtr->u = (float)(i%2);
		  VertexPtr->v = (float)(j%2);
		  *t_Heights++ = VertexPtr->y;
		  VertexPtr++;

	}

    for (int i = 0; i < (TER_X); i++)
	    for (int j = 0; j < (TER_Y); j++) {
  		  
		    DWORD te = i*(TER_Y+1) + j;

		    *IndexPtr++ = te;
		    *IndexPtr++ = te+1;

		    *IndexPtr++ = te + TER_Y + 1;
		    *IndexPtr++ = te + TER_Y + 2; 

		    *IndexPtr++ = te + TER_Y + 1;
		    *IndexPtr++ = te + 1;
      }
	g_TerrainMesh->UnlockIndexBuffer();
  g_TerrainMesh->UnlockVertexBuffer();	

  g_TerrainMesh->OptimizeInplace(
                            D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                            NULL, NULL, NULL, NULL);
	D3DXComputeNormals(g_TerrainMesh,NULL);
}

cTerrain::~cTerrain(void)
{
  m_tertex[0]->Release();
	g_TerrainMesh->Release();
  delete [] m_Heights;
}

float cTerrain::GetHeight(float x, float y)
{
 x = x/(TER_WIDTH+1) + (TER_X+1)/2;
 y = y/(TER_WIDTH+1) + (TER_Y+1)/2;
 if (x > 0 && y > 0 && x < (TER_X + 1) && y < (TER_Y+1))
 return m_Heights[(int)x* (TER_Y + 1) + (int)y];
 else return 1.0f;
}