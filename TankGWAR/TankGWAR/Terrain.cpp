#include "StdAfx.h"
#include "terrain.h"
#include <d3dx9.h>
#include <stdio.h>

LPDIRECT3DTEXTURE9*	  cTerrain::m_tertex;

cTerrain::cTerrain(void)
{
	D3DVERTEXELEMENT9 vertelement;

	vertelement.Stream = 0;
	vertelement.Offset = 0;
	vertelement.Type = D3DDECLTYPE_FLOAT3;
	vertelement.Method = D3DDECLMETHOD_DEFAULT;
	_Init();
/*

	D3DXCreateMesh(
		1,   // NumFaces
		3,   // NumVertices
		0,	 // Options

*/
}

void cTerrain::Paint()
{
	D3DXMATRIX matwrld;
	D3DXMatrixIdentity(&matwrld);
	g_D3DObject->m_d3ddevice9->SetTransform(D3DTS_WORLD,&matwrld);
	if (g_TerrainMesh != NULL)
  {    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = 1.0f;
    mtrl.Diffuse.g = 1.0f;
    mtrl.Diffuse.b = 1.0f;
    mtrl.Diffuse.a = 1.0f;
    mtrl.Specular = mtrl.Diffuse;
    mtrl.Power = 100.0f;
    g_D3DObject->m_d3ddevice9->SetMaterial( &mtrl );

    g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
	  g_TerrainMesh->DrawSubset(0);
  }
}

void cTerrain::_Init()
{


	// Create the mesh	
    /*
    D3DXCreateMeshFVF( 2, 4, D3DXMESH_32BIT | D3DXMESH_SYSTEMMEM, D3DFVF_MESH, g_D3DObject->m_d3ddevice9, &g_TerrainMesh );	
       sCustomVertex Verts[4] = {
	     { -100.0f, 0.0f, 100.0f,  0.0f, 1.0f, 0.0f, D3DCOLOR_RGBA(255,0,255,255), 0.0f, 0.0f },
	     { 100.0f,  0.0f, 100.0f,  0.0f, 1.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 1.0f, 0.0f },
	     { 100.0f,  0.0f, -100.0f, 0.0f, 1.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 1.0f, 1.0f },
	     { -100.0f,  0.0f, -100.0f,0.0f, 1.0f, 0.0f, D3DCOLOR_RGBA(255,255,255,255), 0.0f, 1.0f },
     };
     */
  
    D3DXCreateMeshFVF( 2*TER_X*TER_Y, (TER_X + 1) * (TER_Y + 1) , 
	  D3DXMESH_32BIT | D3DXMESH_SYSTEMMEM, 
	  D3DFVF_MESH, 
	  g_D3DObject->m_d3ddevice9, 
	  &g_TerrainMesh 
	);

	m_Heights = new float[(TER_X + 1) * (TER_Y + 1)];
    //sCustomVertex *Vert = new sCustomVertex[(TER_X + 1) * (TER_Y + 1)];
	
	LPDIRECT3DVERTEXBUFFER9 pTetVB = NULL; //Pointer to vertex buffer.


  char texpath[1024];
  LPDIRECT3DTEXTURE9 tempt;

  m_tertex = new LPDIRECT3DTEXTURE9 [1];
  sprintf (texpath,"resource\\%s","texture2.bmp");
  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
    return;
  m_tertex[0] = tempt;

	RandomizeMesh();


    //DWORD IndexData[6] = { 0, 1, 3, 1, 2, 3 };
    

   // memcpy( VertexPtr, Vert, (TER_X + 1) * (TER_Y + 1)*sizeof(sCustomVertex) );	
    //g_TerrainMesh->LockIndexBuffer( 0, &IndexPtr );	
    //memcpy( IndexPtr, IndexData, 6*sizeof(DWORD) );	
    

// Save the mesh to an X file	
//	D3DXSaveMeshToX( "Test.x", g_TerrainMesh, NULL, &Mat, 1, DXFILEFORMAT_TEXT );
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
		  VertexPtr->y = (float) (rand()%65/10);
      //VertexPtr->y = -5;
		  VertexPtr->nx = 0;
		  VertexPtr->ny = 1;
		  VertexPtr->nz = 0;
//		  VertexPtr->diffuse = randcolor();
		  //VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,0,255);
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
}
