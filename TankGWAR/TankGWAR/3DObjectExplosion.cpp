#include "StdAfx.h"
#include ".\3dobjectexplosion.h"
#include <stdio.h>

int             c3DObjectExplosion::m_graph_init = 0;
LPD3DXMESH			c3DObjectExplosion::m_expmesh;
LPDIRECT3DTEXTURE9*	c3DObjectExplosion::m_exptex;
D3DMATERIAL9*		c3DObjectExplosion::m_expmat;
DWORD				    c3DObjectExplosion::m_expNmat;


c3DObjectExplosion::c3DObjectExplosion(void)
{
	if (!m_graph_init++) _LoadGraphics();

  m_curmesh = m_expmesh;
  m_nMat = m_expNmat;
  m_curtex = m_exptex;
  m_curmat = m_expmat;
  m_exptime.Reset();
}

c3DObjectExplosion::~c3DObjectExplosion(void)
{
   if (!--m_graph_init) _UnloadGraphics();
}

void c3DObjectExplosion::_UnloadGraphics(void)
{
  SAFE_RELEASE(m_expmesh);
  for (int x = 0; x < (int)m_nMat; x++) {
	  SAFE_RELEASE(m_exptex[x]);
  }
  SAFE_DELETE_ARRAY(m_exptex);
  SAFE_DELETE_ARRAY(m_expmat);
}

void c3DObjectExplosion::_LoadGraphics(void)
{
	LPD3DXBUFFER lpMat = NULL;
  LPD3DXBUFFER pAdjacencyBuffer = NULL;

  if (FAILED(D3DXCreateSphere(
     g_D3DObject->m_d3ddevice9,
     10.0f,         // Radius
     30,         // Slices
     30,         // Stacks
     &m_expmesh,
     &pAdjacencyBuffer		// LPD3DXBUFFER *ppAdjacency,
     )))
     m_expmesh = NULL;
   else
     if( FAILED( m_expmesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
    {
        SAFE_RELEASE( pAdjacencyBuffer );
        return;
    }

   m_expNmat = 1;
   m_exptex = new LPDIRECT3DTEXTURE9 [m_expNmat];
   m_expmat = new D3DMATERIAL9 [m_expNmat];

   m_expmat[0].Ambient = D3DXCOLOR(0.0f,0.0f,0.0f,0.2f);
   m_expmat[0].Diffuse = D3DXCOLOR(1.0f,0.0f,0.1f,0.8f);
   m_expmat[0].Specular = D3DXCOLOR(1.0f,1.0f,1.0f,0.5f);
   m_expmat[0].Emissive = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
   m_expmat[0].Power = 1.0f;

   m_exptex[0] =NULL;
}

void c3DObjectExplosion::paint(void)
{
  m_scale = D3DXVECTOR3(1,1,1) * m_exptime.PeekTime()/6000;
  char debg[255];
  sprintf (debg,"%.5f",m_exptime.PeekTime()/1000);
  g_D3DObject->DrawTextStr(150,150,D3DCOLOR_XRGB(255,240,230),debg);

   for (int x = 0; x < (int)m_nMat; x++)  {
     MakeWorldMatrix(x);
     if (m_curtex)
     g_D3DObject->m_d3ddevice9->SetTexture(0,m_curtex[x]);
     g_D3DObject->m_d3ddevice9->SetMaterial(&m_curmat[x]);
     m_curmesh->DrawSubset(0);
   }
}