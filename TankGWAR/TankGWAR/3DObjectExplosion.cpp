#include "StdAfx.h"
#include ".\3dobjectexplosion.h"
#include "wavsound.h"
#include <stdio.h>

int             c3DObjectExplosion::m_graph_init = 0;
LPD3DXMESH			c3DObjectExplosion::m_expmesh;
LPDIRECT3DTEXTURE9*	c3DObjectExplosion::m_exptex;
D3DMATERIAL9*		c3DObjectExplosion::m_expmat;
DWORD				    c3DObjectExplosion::m_expNmat;
extern cwavsound *wav;

c3DObjectExplosion::c3DObjectExplosion(float radius,float exp_dur, D3DCOLORVALUE col)
{
	if (!m_graph_init++) _LoadGraphics();

  m_curmesh = m_expmesh;
  m_nMat = m_expNmat;
  m_curtex = m_exptex;
  m_curmat = m_expmat;
  m_exptime.Reset();

  m_expdur = exp_dur;
  m_radius = radius;
  m_expcolor =col;
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
     1.0f,         // Radius
     60,         // Slices
     60,         // Stacks
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
   wav->play(explosion);
   m_expNmat = 1;
   m_exptex = new LPDIRECT3DTEXTURE9 [m_expNmat];
   m_expmat = new D3DMATERIAL9 [m_expNmat];

   m_expmat[0].Ambient = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
   m_expmat[0].Diffuse = m_expcolor;
   m_expmat[0].Specular = D3DXCOLOR(1.0f,1.0f,1.0f,0.5f);
   m_expmat[0].Emissive = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
   m_expmat[0].Power = 1.0f;

   m_exptex[0] =NULL;
}

float cutone (float m)
{
  if (m > 1)
    return 1;
  return m;
}
void c3DObjectExplosion::paint(void)
{
  m_scale = D3DXVECTOR3(1,1,1) * m_radius * cutone(m_exptime.PeekTime()/m_expdur);
//  char debg[255];
//  sprintf (debg,"%.5f",m_exptime.PeekTime()/1000);
//  g_D3DObject->DrawTextStr(150,150,D3DCOLOR_XRGB(255,240,230),debg);

//  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,FALSE);
   for (int x = 0; x < (int)m_nMat; x++)  {
     MakeWorldMatrix(x);
     if (m_curtex)
       g_D3DObject->m_d3ddevice9->SetTexture(0,m_curtex[x]);
 //    m_expmat[x].Ambient = m_expcolor;
     m_expmat[x].Diffuse = m_expcolor;
     g_D3DObject->m_d3ddevice9->SetMaterial(&m_curmat[x]);
     m_curmesh->DrawSubset(0);
   }
//  g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_LIGHTING,TRUE);
}