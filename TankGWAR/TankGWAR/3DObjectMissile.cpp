#include "StdAfx.h"
#include "3dobjectmissile.h"
#include <stdio.h>

LPD3DXMESH			c3DObjectMissile::m_missilemesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_missiletex;
D3DMATERIAL9*		c3DObjectMissile::m_missilemat;
DWORD           c3DObjectMissile::m_missileNmat;

int c3DObjectMissile::m_graph_init = 0;

void c3DObjectMissile::move()
{
  //m_orient.x = sin(m_velocity.y/m_initYvelocity);
  //m_orient.z = cos(m_velocity.y/m_initYvelocity);
  m_orient.x = -sin(m_velocity.y/m_initYvelocity) * D3DX_PI/2;
  c3DObject::move();
}

c3DObjectMissile::c3DObjectMissile(void)
{
	if (!m_graph_init++) _LoadGraphics();

  m_curmesh = m_missilemesh;
  m_nMat = m_missileNmat;
  m_curtex = m_missiletex;
  m_curmat = m_missilemat;
  m_time.Reset();

  m_position = D3DXVECTOR3(0,0,0.0f);
  m_velocity = D3DXVECTOR3((rand()%80/10)-4,9,5);
  m_accel    = D3DXVECTOR3(0,-3,0);
  m_orient   = D3DXVECTOR3(0,0,0);

  m_initYvelocity = m_velocity.y;
}

c3DObjectMissile::~c3DObjectMissile(void)
{
	if (!--m_graph_init) _UnloadGraphics();
}


void c3DObjectMissile::_UnloadGraphics()
{
  m_missilemesh->Release();
  for (int x = 0; x < (int)m_nMat; x++) {
	  SAFE_RELEASE(m_missiletex[x]);
  }
  SAFE_DELETE_ARRAY(m_missiletex);
  SAFE_DELETE_ARRAY(m_missilemat);
}
void c3DObjectMissile::_LoadGraphics()
{
	LPD3DXBUFFER lpMat = NULL;
    LPD3DXBUFFER pAdjacencyBuffer = NULL;
	HRESULT hr;

  if (FAILED(D3DXLoadMeshFromX(
     "resource\\scud.X",
     D3DXMESH_SYSTEMMEM,
	 g_D3DObject->m_d3ddevice9,
     &pAdjacencyBuffer,		// LPD3DXBUFFER *ppAdjacency,
     &lpMat,				// LPD3DXBUFFER *ppMaterials,
     NULL,					// LPD3DXBUFFER *ppEffectInstances,
     &m_missileNmat,				// DWORD *pNumMaterials,
     &m_missilemesh
   )))
     m_missilemesh = NULL;
   else
     if( FAILED( hr = m_missilemesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
    {
        SAFE_RELEASE( pAdjacencyBuffer );
        SAFE_RELEASE( lpMat );
        return;
    }

   D3DXMATERIAL* mat = (D3DXMATERIAL*) lpMat->GetBufferPointer();
   char texpath[512];

   m_missiletex = new LPDIRECT3DTEXTURE9 [m_missileNmat];
   m_missilemat = new D3DMATERIAL9 [m_missileNmat];
 
   for (DWORD x = 0; x < m_missileNmat; x++) {
	   LPDIRECT3DTEXTURE9 tempt;
	   tempt = NULL;

     m_missilemat[x] = mat[x].MatD3D;
/*
     m_missilemat[x].Ambient.a = 
     m_missilemat[x].Ambient.r = 
     m_missilemat[x].Ambient.g = 
     m_missilemat[x].Ambient.b = 70;
*/
     m_missilemat[x].Ambient.r = 
     m_missilemat[x].Ambient.g = 
     m_missilemat[x].Ambient.b = 0.3;
     //m_missilemat[x].Diffuse = m_missilemat[x].Ambient;
     m_missilemat[x].Specular.r = 
     m_missilemat[x].Specular.g = 
     m_missilemat[x].Specular.b = 0.5;
     //m_missilemat[x].Emissive = m_missilemat[x].Ambient;

//     m_missilemat[x].Power = 100.0f;
	   m_missiletex[x] = NULL;
	   if (mat[x].pTextureFilename != NULL) {               
		        sprintf (texpath,"resource\\%s",mat[x].pTextureFilename);
	       if (hr = D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
		   {
			   m_missiletex[x] = NULL;
			   OutputDebugString("Create Texture Failed\n");
		   }
		   else {
			   m_missiletex[x] = tempt;
		   }
	   }     
   }
    SAFE_RELEASE( pAdjacencyBuffer );
    //SAFE_RELEASE( lpMat );
}