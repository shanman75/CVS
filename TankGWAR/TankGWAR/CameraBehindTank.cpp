#include "StdAfx.h"
#include "camerabehindtank.h"
#include "GameState.h"
#include <stdio.h>

cCameraBehindTank::cCameraBehindTank(void)
{
}

cCameraBehindTank::~cCameraBehindTank(void)
{
}


void cCameraBehindTank::SetCamera()
{
    D3DXMATRIXA16 matView;
    float ht;

    D3DXVECTOR3 tpos, tor;

    g_GameState->GetCurrentTankState(&tpos, &tor);

    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( tpos.x - 8*sin(tor.x), tpos.y + 2, tpos.z -8*cos(tor.x) );
    if ((ht = (g_GameState->GetTerrainHeight(vFromPt.x,vFromPt.z)+1.0f)) > vFromPt.y)
    {
      char deb[512];
      sprintf(deb, "Height to low, adjusting down (%.1f,%.1f) - Height was %.2f  Height is %.2f\n",
        vFromPt.x, vFromPt.z,
        vFromPt.y, ht);
      OutputDebugString(deb);
      vFromPt.y = ht+1.0f;
    }
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( tpos.x, tpos.y+(tor.y)*3.6f, tpos.z );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );


    D3DXMATRIXA16 matProj;
    FLOAT fAspect = ((FLOAT)WIDTH/HEIGHT);
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 0.1f, 54350.0f );
	  g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );   
}