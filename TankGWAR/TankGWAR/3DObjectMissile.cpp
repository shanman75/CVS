#include "StdAfx.h"
#include "3dobjectmissile.h"
#include <stdio.h>

LPD3DXMESH			c3DObjectMissile::m_missilemesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_missiletex;
D3DMATERIAL9*		c3DObjectMissile::m_missilemat;
DWORD				c3DObjectMissile::m_missileNmat;

int c3DObjectMissile::m_graph_init = 0;

float clamp (float z, float q)
{
  if (z > q) return q;
  else if (-z > q) return q;
  else return z;
}

void c3DObjectMissile::move()
{
  //m_orient.x = sin(m_velocity.y/m_initYvelocity);
  //m_orient.z = cos(m_velocity.y/m_initYvelocity);
  //m_orient.x = -sin(m_velocity.y/m_initYvelocity) * D3DX_PI/2;
  //if (m_velocity.y < 0) m_orient.x = clamp(m_orient.x+m_time.PeekTime()/(120*(m_velocity.x+m_velocity.z)),D3DX_PI/2);

  float xzLen = 0;
  xzLen = sqrt(m_velocity.x*m_velocity.x + m_velocity.z*m_velocity.z);
  if (xzLen == 0) { xzLen = 0.001f; }
  
  m_orient.x = clamp(-atan(m_velocity.y/xzLen),D3DX_PI/2);
//  m_orient.y = -atan(m_velocity.y/10);
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
  m_initYvelocity=-999.0f;

//  m_velocity = D3DXVECTOR3((rand()%80/10)-4,9,5);
//  m_accel    = D3DXVECTOR3(0,-3,0);

}

c3DObjectMissile::~c3DObjectMissile(void)
{
	if (!--m_graph_init) _UnloadGraphics();
}


void c3DObjectMissile::_UnloadGraphics()
{
  SAFE_RELEASE(m_missilemesh);
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

  if (FAILED(D3DXLoadMeshFromX(
     "resource\\abomb.x",
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
     if( FAILED( m_missilemesh->OptimizeInplace(
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
//     m_missilemat[x].Diffuse = D3DXCOLOR(0.6f,0.6f,0.4f,1.0f);
//     m_missilemat[x].Ambient = D3DXCOLOR(0.1f,0.1f,0.1f,1.0f);
//     m_missilemat[x].Specular = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
//     m_missilemat[x].Power = 0.2f;
     m_missiletex[x] = NULL;
     if (mat[x].pTextureFilename != NULL) {               
		        sprintf (texpath,"resource\\%s",mat[x].pTextureFilename);
	       if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
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
    SAFE_RELEASE( lpMat );
}