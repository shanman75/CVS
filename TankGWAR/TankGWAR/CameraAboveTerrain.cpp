#include "StdAfx.h"
#include "cameraaboveterrain.h"
#include "GameState.h"

cCameraAboveTerrain::cCameraAboveTerrain(void)
{
}

cCameraAboveTerrain::~cCameraAboveTerrain(void)
{
}


void cCameraAboveTerrain::SetCamera()
{
    D3DXMATRIXA16 matView;

    D3DXVECTOR3 tpos, tor;
    float tzoom;

    g_GameState->GetCurrentTankState(&tpos, &tor);
    g_GameState->GetCurrentCamAboveZoom(&tzoom);

    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( tpos.x, tpos.y + 80.0f*tzoom, tpos.z );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( tpos.x+0.1f, tpos.y, tpos.z );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );


    D3DXMATRIXA16 matProj;
    FLOAT fAspect = ((FLOAT)WIDTH/HEIGHT);
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/2, fAspect, 0.1f, 54350.0f );
	  g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );   
}