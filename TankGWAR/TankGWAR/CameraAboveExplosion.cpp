#include "StdAfx.h"
#include "cameraaboveexplosion.h"
#include "GameState.h"

cCameraAboveExplosion::cCameraAboveExplosion(void)
{
}

cCameraAboveExplosion::~cCameraAboveExplosion(void)
{
}

void cCameraAboveExplosion::SetCamera()
{
    D3DXMATRIXA16 matView;
    D3DXVECTOR3 tpos, tscale;

    g_GameState->GetCurrentExpState(&tpos, &tscale);

//    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( tpos.x - 8*sin(tor.x), tpos.y + 2, tpos.z -8*cos(tor.x) );
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( tpos.x + cos(D3DX_PI*2*tscale.x)*20.0f, tpos.y + 10.0f, 
                                      tpos.z+sin(D3DX_PI*2*tscale.z)*20.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( tpos.x, tpos.y, tpos.z );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );


    D3DXMATRIXA16 matProj;
    FLOAT fAspect = ((FLOAT)WIDTH/HEIGHT);
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 0.1f, 50250.0f );
	  g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );   
}