#include "StdAfx.h"
#include "camera.h"


cCamera::cCamera(void)
{
	m_x = 0;
	m_y = 0;
	m_z = -5;
	m_time.Reset();
	m_time.setInterval(0.2);

}

cCamera::~cCamera(void)
{
}

void cCamera::event(EVENT evnt)
{
  OutputDebugString("Got a camera event\n");
  if (m_time.CmpTime())
  switch (evnt) {
	case LEFT:  m_x -= 0.3f; break;
	case RIGHT: m_x += 0.3f; break;
	case DOWN:  m_z += 0.3f; break;
	case UP:    m_z -= 0.3f; break;
	default: break;
  }
}

void cCamera::SetCamera(void)
{
    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_x, m_y, m_z );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );


    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)800) / 600;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );
	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );
}