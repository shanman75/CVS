#include "StdAfx.h"
#include "terrain.h"
#include <d3dx9.h>
#include <stdio.h>

LPDIRECT3DTEXTURE9*	  cTerrain::m_tertex;

cTerrain::cTerrain(float x,float z,float w)
{
  m_tertex = NULL;

  TER_X = x;  TER_Z = z; 
  TER_WIDTH = w;   // Ignored
  m_Heights = new float[(TER_X+1)*(TER_Z+1)];
  for (int x = 0; x < (TER_X+1)*(TER_Z+1); x++)
    m_Heights[x] = rand()%900/15;
  
  // Pass 1
  for (int t = 1; t < 3; t++)
  for (int i = 1; i < TER_X-1; i++)
  for (int j = 1; j < TER_Z-1; j++)
    m_Heights[(i)*(int)TER_X+j] = 
                   ( m_Heights[(i)*(int)TER_X+j-1] + m_Heights[(i)*(int)TER_X+j] + 
                     m_Heights[(i)*(int)TER_X+j+1] +0
                    )/3;
  // Pass 2
  for (int t = 1; t < 6; t++)
  for (int i = 1; i < TER_X-1; i++)
  for (int j = 1; j < TER_Z-1; j++)
    m_Heights[(i)*(int)TER_X+j] = 
                   ( 
                     m_Heights[(i-1)*(int)TER_X+j] + m_Heights[(i+1)*(int)TER_X+j] +
                     m_Heights[(i)*(int)TER_X+j+1] +0
                    )/3;
	_Init();
}

void cTerrain::Paint()
{
	D3DXMATRIXA16 matwrld;
  D3DMATERIAL9 mtrl;
  ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
	D3DXMatrixIdentity(&matwrld);
	g_D3DObject->m_d3ddevice9->SetTransform(D3DTS_WORLD,&matwrld);
  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,FALSE);

  if (g_TerrainMesh != NULL)
  {   
    g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
//    g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	  g_TerrainMesh->DrawSubset(0);
    g_TerrainMeshBig->DrawSubset(0);
  }
  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,TRUE);
}

void cTerrain::_Init()
{

  // Create the smaller VB
  if (FAILED(D3DXCreateMeshFVF(
    2*(TER_X_SMALL)*(TER_Z_SMALL), 
    (TER_X_SMALL+1) * (TER_Z_SMALL+1), 
    //D3DPOOL_DEFAULT | D3DXMESH_32BIT,  //	  
    D3DXMESH_32BIT | D3DXMESH_MANAGED, 
	  D3DFVF_MESH,
	  g_D3DObject->m_d3ddevice9, 
	  &g_TerrainMesh 
	))) 
    exit(1);

  if (FAILED(D3DXCreateMeshFVF(
    2*(TER_X_BIG)*(TER_Z_BIG), 
    (TER_X_BIG+1) * (TER_Z_BIG+1), 
    //D3DPOOL_DEFAULT | D3DXMESH_32BIT,  //	  
    D3DXMESH_32BIT | D3DXMESH_MANAGED, 
	  D3DFVF_MESH,
	  g_D3DObject->m_d3ddevice9, 
	  &g_TerrainMeshBig
	)))
      exit(1);

	LPDIRECT3DVERTEXBUFFER9 pTetVB = NULL; //Pointer to vertex buffer.

  char texpath[1024];
  LPDIRECT3DTEXTURE9 tempt;

  m_tertex = new LPDIRECT3DTEXTURE9 [1];
  sprintf (texpath,"resource\\%s","BUMP_Dents_bp.dds");
//  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
//    return;
  if (FAILED(D3DXCreateTextureFromFileEx(
                                  g_D3DObject->m_d3ddevice9,    // Device
                                  texpath,
                                  D3DX_DEFAULT,D3DX_DEFAULT,    // Height/Width
                                  D3DX_DEFAULT,                 // MIP Maps
                                  0,                            // Usage
                                  D3DFMT_A8R8G8B8,              // Format
                                  D3DPOOL_DEFAULT,              // Pool
                                  D3DX_DEFAULT,                 // Filter
                                  D3DX_DEFAULT,                 // MIP Filter (Default = BOX)
                                  0,                            // Color Key (0 = disabled)
                                  NULL,                         // Ptr to SRCINFO
                                  NULL,                         // Palette entry
                                  &tempt                       // Ptr to Texture
                                  )))
                                  exit(0);
  m_tertex[0] = tempt;

	RandomizeMesh();
}

D3DCOLOR randcolor()
{
	switch (rand()%3) {
		case 3:
			return D3DCOLOR_RGBA(36,171,5,255); break;
		case 2:
			return D3DCOLOR_RGBA(245,210,3,255); break;
		case 1:
			return D3DCOLOR_RGBA(38,190,3,255); break;
		default:
			return D3DCOLOR_RGBA(38,100,3,255); break;
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

	IndexPtr =  NULL;
	VertexPtr= NULL;
  
  if(FAILED(g_TerrainMesh->LockVertexBuffer( 0, (LPVOID *) &VertexPtr )))
    return;
  if(FAILED(g_TerrainMesh->LockIndexBuffer( 0, (LPVOID *) &IndexPtr )))
    return;
  for (int i = 0; i < TER_X_SMALL+1; i++)
	  for (int j = 0; j < TER_Z_SMALL+1; j++) {
		  VertexPtr->x = (i - (TER_X_SMALL-1)/2) * TER_WIDTH_SMALL;
		  VertexPtr->z = (j - (TER_Z_SMALL-1)/2) * TER_WIDTH_SMALL;
		  //VertexPtr->y = (float) (rand()%55/5)+33/TER_WIDTH;
      VertexPtr->y = GetHeight(VertexPtr->x,VertexPtr->z);
		  VertexPtr->diffuse = randcolor();
//		  VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,255,255);
 		  VertexPtr->u = (float)(i%2);
		  VertexPtr->v = (float)(j%2);
//      SetHeight(VertexPtr->x,VertexPtr->z,VertexPtr->y);
		  VertexPtr++;
	}
    for (int i = 0; i < (TER_X_SMALL); i++)
	    for (int j = 0; j < (TER_Z_SMALL); j++) {  		  
		    DWORD te = i*(TER_Z_SMALL+1) + j;

		    *IndexPtr++ = te;
		    *IndexPtr++ = te+1;

		    *IndexPtr++ = te + TER_Z_SMALL + 1;
		    *IndexPtr++ = te + TER_Z_SMALL + 2; 

		    *IndexPtr++ = te + TER_Z_SMALL + 1;
		    *IndexPtr++ = te + 1;
      }
	g_TerrainMesh->UnlockIndexBuffer();
  g_TerrainMesh->UnlockVertexBuffer();	

	IndexPtr =  NULL;
	VertexPtr= NULL;

  if(FAILED(g_TerrainMeshBig->LockVertexBuffer( 0, (LPVOID *) &VertexPtr )))
    return;
  if(FAILED(g_TerrainMeshBig->LockIndexBuffer( 0, (LPVOID *) &IndexPtr )))
    return;
  for (int i = 0; i < (TER_X_BIG+1); i++)
	  for (int j = 0; j < (TER_Z_BIG+1); j++) {
		  VertexPtr->x = (i - (TER_X_BIG-1)/2) * TER_WIDTH_BIG;
		  VertexPtr->z = (j - (TER_Z_BIG-1)/2) * TER_WIDTH_BIG;
		  //VertexPtr->y = (float) (rand()%55/5)+33/TER_WIDTH;
      if (fabs(VertexPtr->x) > (TER_X_SMALL+1)/2*TER_WIDTH_SMALL ||
          fabs(VertexPtr->z) > (TER_Z_SMALL+1)/2*TER_WIDTH_SMALL)
        VertexPtr->y = min(GetHeight(VertexPtr->x,VertexPtr->z),1.0f);
      else 
        VertexPtr->y = -150.0f;
		  VertexPtr->diffuse = randcolor();
//		  VertexPtr->diffuse = D3DCOLOR_RGBA(255,255,255,255);
 		  VertexPtr->u = (float)(i%2);
		  VertexPtr->v = (float)(j%2);
//      SetHeight(VertexPtr->x,VertexPtr->z,VertexPtr->y);
		  VertexPtr++;
	}

    for (int i = 0; i < (TER_X_BIG); i++)
	    for (int j = 0; j < (TER_Z_BIG); j++) {  		  
		    DWORD te = i*(TER_Z_BIG+1) + j;

		    *IndexPtr++ = te;
		    *IndexPtr++ = te+1;

		    *IndexPtr++ = te + TER_Z_BIG + 1;
		    *IndexPtr++ = te + TER_Z_BIG + 2; 

		    *IndexPtr++= te + TER_Z_BIG + 1;
		    *IndexPtr++ = te + 1;
      }
	g_TerrainMeshBig->UnlockIndexBuffer();
  g_TerrainMeshBig->UnlockVertexBuffer();
//  g_TerrainMesh->OptimizeInplace(
//                            D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
//                            NULL, NULL, NULL, NULL);
//	D3DXComputeNormals(g_TerrainMesh,NULL);
}

cTerrain::~cTerrain(void)
{
  OnLostDevice();
  delete [] m_Heights;
}

void cTerrain::OnLostDevice(void)
{
  SAFE_RELEASE(m_tertex[0]);
	SAFE_RELEASE(g_TerrainMesh);
	SAFE_RELEASE(g_TerrainMeshBig);
}

void cTerrain::OnResetDevice(void)
{
  _Init();
}

float cTerrain::GetHeight(float x, float z)
{
  //return 1;
 x = x + TER_X/2;
 z = z + TER_Z/2;
 if (!(x > 0 && z > 0 && x < (TER_X - 1) && z < (TER_Z-1)))
    return 25999.0f;   // Default Value

 // Find triangle face
 D3DXVECTOR3 pt1, pt2, pt3;
// float       wt1, wt2, wt3;
 float       dt1, dt2, dt3;

 return m_Heights[(int)(floor(x)*TER_Z + floor(z))]; 

 pt1 = D3DXVECTOR3(floor(x),0,floor(z));
 pt3 = D3DXVECTOR3(pt1.x+1,0,pt1.z+1);

 if (fmod(x,1) > (1-fmod(z,1)))
    pt2 = D3DXVECTOR3(pt1.x+1,0,pt1.z);
 else
    pt2 = D3DXVECTOR3(pt1.x,  0,pt1.z+1);

 pt1.y = m_Heights[(int)(pt1.x*TER_Z + pt1.z)]; 
 pt2.y = m_Heights[(int)(pt2.x*TER_Z + pt2.z)]; 
 pt3.y = m_Heights[(int)(pt3.x*TER_Z + pt3.z)]; 

 dt1 = sqrt((pt1.x-x)*(pt1.x-x) + (pt1.z-z)*(pt1.z-z));
 dt2 = sqrt((pt2.x-x)*(pt2.x-x) + (pt2.z-z)*(pt2.z-z));
 dt3 = sqrt((pt3.x-x)*(pt3.x-x) + (pt3.z-z)*(pt3.z-z));

 float height = (pt1.y * dt1 + pt2.y * dt2 + pt3.y * dt3)/3;
 return height;
}

void cTerrain::SetHeight(float x, float z, float y)
{
  return;
 x = x/(TER_WIDTH) + TER_X/2;
 z = z/(TER_WIDTH) + TER_Z/2;

 if (x > 0 && z > 0 && x < TER_X && z < TER_Z)
    m_Heights[(int)(x* TER_Z + z)] = y;
}
