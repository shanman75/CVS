#include "StdAfx.h"
#include "terrain.h"
#include <d3dx9.h>

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
	  g_TerrainMesh->DrawSubset(0);
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

	RandomizeMesh();


    //DWORD IndexData[6] = { 0, 1, 3, 1, 2, 3 };
    

   // memcpy( VertexPtr, Vert, (TER_X + 1) * (TER_Y + 1)*sizeof(sCustomVertex) );	
    //g_TerrainMesh->LockIndexBuffer( 0, &IndexPtr );	
    //memcpy( IndexPtr, IndexData, 6*sizeof(DWORD) );	
    

// Save the mesh to an X file	
//	D3DXSaveMeshToX( "Test.x", g_TerrainMesh, NULL, &Mat, 1, DXFILEFORMAT_TEXT );
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

	g_TerrainMesh->LockVertexBuffer( 0, (LPVOID *) &VertexPtr );	
	g_TerrainMesh->LockIndexBuffer( 0, (LPVOID *) &IndexPtr );	

    for (int i = 0; i < (TER_X + 1); i++)
	  for (int j = 0; j < (TER_Y + 1); j++) {
		  VertexPtr->x = (i - (TER_X-1)/2) * TER_WIDTH;
		  VertexPtr->z = (j - (TER_Y-1)/2) * TER_WIDTH;
		  VertexPtr->y = (float) (rand()%200/10)-15;
		  //VertexPtr->y = -0.5;
		  VertexPtr->nx = 0;
		  VertexPtr->ny = 1;
		  VertexPtr->nz = 0;
		  VertexPtr->diffuse = D3DCOLOR_RGBA(rand()%25,rand()%255,rand()%255,255);
		  //VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,0,255);
 		  VertexPtr->u = i%2;
		  VertexPtr->v = j%2;
		  *m_Heights++ = VertexPtr->y;
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

//		  *IndexPtr++ = 0;
//		  *IndexPtr++ = 1;
//		  *IndexPtr++ = 2;
//		  *IndexPtr++ = 3;
//		  *IndexPtr++ = 2;
//		  *IndexPtr++ = 1;
    }
	g_TerrainMesh->UnlockIndexBuffer();
    g_TerrainMesh->UnlockVertexBuffer();	

//	D3DXComputeNormals(g_TerrainMesh,NULL);
}

cTerrain::~cTerrain(void)
{
	g_TerrainMesh->Release();
}
