#include "StdAfx.h"
#include "camera.h"
#include "terrain.h"
#include <stdio.h>

extern cTerrain *terrain;


cCamera::cCamera(void)
{
	m_x = 0;
	m_y = 18.0f;
	m_z = -19.0f;

	m_rotx = 0;
	m_roty = 0;
	m_zoom = 3;

	m_time.Reset();
	m_time.setInterval(0.2f);

}

cCamera::~cCamera(void)
{
}

void cCamera::event(EVENT evnt)
{
  OutputDebugString("Got a camera event\n");
  if (m_time.CmpTime())
  switch (evnt) {
	case LEFT:  m_z -= 0.4f; break;
	case RIGHT: m_z += 0.4f; break;
	case DOWN:  m_y += 0.4f; break;
	case UP:    m_y -= 0.4f; break;
	case ZOOMIN: m_zoom -= 0.8f; break;
	case ZOOMOUT: m_zoom += 0.8f; break;
	default: break;
  }


  /*
  if (m_roty > D3DX_PI/2) {
	  m_rotx = m_rotx - D3DX_PI;
  }
  if (m_roty < -D3DX_PI/2) {
	  m_roty = m_roty+D3DX_PI;
  }
  if (m_rotx > D3DX_PI*2) 
	  m_rotx = m_rotx-2*D3DX_PI;
  */

}

void cCamera::SetCamera(void)
{
    D3DXMATRIX matView;

/*
	if (fmod(m_roty,D3DX_PI) > D3DX_PI/2) {
		m_z = (cos(m_roty) * cos(m_rotx)) * m_zoom;
		//(sin(m_roty) + cos(m_rotx)) * m_zoom;
		m_x = (cos(m_roty) * sin(m_rotx)) * m_zoom;
	}
	else {
	*/ {
		//m_x = (cos(m_roty) * cos(m_rotx)) * m_zoom;
		//(sin(m_roty) + cos(m_rotx)) * m_zoom;
		//m_z = (cos(m_roty) * sin(m_rotx)) * m_zoom;
	}
	//m_y = sin(m_roty) * m_zoom;

	/*
	m_x = m_zoom * ( 1/cosh(m_roty) * cos(m_rotx));
	m_y = m_zoom * ( 1/cosh(m_roty) * sin(m_rotx));
	m_z = m_zoom * ( tanh(m_roty) );
   */

	//m_y = terrain->GetHeight(m_x,m_z)+10.0f;
	//m_y = 19.2f;
	
	
    /*
	char debg[255];
	sprintf (debg, "Y = %.2f - X = %.2f ZM = %.1f",m_y, m_rotx,m_zoom);
    g_D3DObject->DrawTextStr(300,450,D3DCOLOR_XRGB(240,0,50),debg);
	*/

    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( m_x, m_y, m_z );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 8.0f, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );


    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)WIDTH) / HEIGHT;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 250.0f );
	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );
}
