#include "StdAfx.h"
#include "camerabehindmissile.h"
#include "gamestate.h"
#include <stdio.h>

cCameraBehindMissile::cCameraBehindMissile(void)
{
}

cCameraBehindMissile::~cCameraBehindMissile(void)
{
}

void cCameraBehindMissile::SetCamera()
{
    D3DXMATRIXA16 matView;
    float ht;

    D3DXVECTOR3 tpos, tor, tvel;

    g_GameState->GetCurrentMissileState(&tpos, &tor, &tvel);

    char bah[1255];

    sprintf(bah,"Orientation (%.1f,%.1f) sin,cos(orx) (%.2f,%.2f)  sin,cos(ory) (%.2f,%.2f) (%.2f,%.2f,%.2f\n", 
                  tor.x, tor.y,
                  sin(tor.x), cos(tor.x),
                  sin(tor.y), cos(tor.y),
                  tpos.x, tpos.y, tpos.z
            );
    OutputDebugString(bah);

    //g_D3DObject->DrawTextStr(60,60,D3DCOLOR_XRGB(4,50,250),"Blah");

    D3DXVECTOR3 vFromPt, vLookatPt, vTemp;
    //vFromPt   = D3DXVECTOR3( tpos.x - cos(tor.y)*8, tpos.y+sin(tor.x)*12, tpos.z - sin(tor.y)*8 );

//    float tsta = 4.0f;
    vFromPt   = D3DXVECTOR3( tpos.x + sin(-tor.y)*cos(tor.x)*2, 
                             tpos.y +            sin(tor.x)*2, 
                             tpos.z + cos(-tor.y)*cos(tor.x)*2 
                            );
    vFromPt   = tpos - *D3DXVec3Normalize(&vTemp,&tvel)*5.6f;
    vFromPt.y += 7.0f;
    if ((ht = (g_GameState->GetTerrainHeight(vFromPt.x,vFromPt.z)+0.5f)) > vFromPt.y)
      vFromPt.y = ht+0.5f;
    vLookatPt = D3DXVECTOR3( tpos.x-0.1f, tpos.y-0.1f, tpos.z-0.1f );

    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
	
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIXA16 matProj;
    FLOAT fAspect = ((FLOAT)WIDTH) / HEIGHT;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 0.1f, 68850.0f );
	  g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );   
}