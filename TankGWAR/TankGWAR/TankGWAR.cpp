// TankGWAR.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TankGWAR.h"
#include "MainWnd.h"
#include "Timer.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#include "D3DObject.h"
#include "Texture.h"
#include "ObjMgr.h"

CTimer time, time2;
CTexture *tex1;
LPD3DXMESH teapot = NULL;
DWORD nMat;
LPDIRECT3DTEXTURE9* tanktex;
D3DMATERIAL9* tankmat;


void g_MainDestroy()
{
  delete tex1;
  tex1 = NULL;
  teapot->Release();
  for (int x = 0; x < nMat; x++) {
	  SAFE_RELEASE(tanktex[x]);
  }
  SAFE_DELETE_ARRAY(tanktex);
  SAFE_DELETE_ARRAY(tankmat);
}
void g_MainInit()
{
    LPD3DXBUFFER lpMat = NULL;
    LPD3DXBUFFER pAdjacencyBuffer = NULL;
	HRESULT hr;

	g_ObjMgr.reset();
   CObj *myobj = new CObj();
   g_ObjMgr.add(myobj);
   tex1 = new CTexture("blah.png",0xFFFF00FF);
   if (FAILED(D3DXLoadMeshFromX(
     "resource\\tank2.x",
     D3DXMESH_SYSTEMMEM,
	 g_D3DObject->m_d3ddevice9,
     &pAdjacencyBuffer,				// LPD3DXBUFFER *ppAdjacency,
     &lpMat,				// LPD3DXBUFFER *ppMaterials,
     NULL,			    // LPD3DXBUFFER *ppEffectInstances,
     &nMat,				// DWORD *pNumMaterials,
     &teapot
   )))
     teapot = NULL;
   else
     if( FAILED( hr = teapot->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
    {
        SAFE_RELEASE( pAdjacencyBuffer );
        SAFE_RELEASE( lpMat );
        return;
    }

   D3DXMATERIAL* mat = (D3DXMATERIAL*) lpMat->GetBufferPointer();
   char texpath[512];

   tanktex = new LPDIRECT3DTEXTURE9 [nMat];
   tankmat = new D3DMATERIAL9 [nMat];
 
   for (DWORD x = 0; x < nMat; x++) {
	   LPDIRECT3DTEXTURE9 tempt;
	   tempt = NULL;
	   tankmat[x] = mat[x].MatD3D;
	   tanktex[x] = NULL;
	   if (mat[x].pTextureFilename != NULL) {               
		        sprintf (texpath,"resource\\%s",mat[x].pTextureFilename);
	       if (hr = D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
		   {
			   tanktex[x] = NULL;
			   OutputDebugString("Create Texture Failed\n");
		   }
		   else {
			   tanktex[x] = tempt;
		   }
	   }
//      g_D3DObject->LoadTextureFromFile(texpath,&tanktex[nMat],0);
   }
    SAFE_RELEASE( pAdjacencyBuffer );
    SAFE_RELEASE( lpMat );
}


void MakeWorldMatrix( D3DXMATRIX* pMatWorld, int x )
{
    D3DXMATRIX MatTemp;  // Temp matrix for rotations.
    D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
                         // pMatWorld.

	float m_xPos = 0;
	float m_yPos = -2;
	static float m_zPos = 5;

	static dir = -1;
	
	m_zPos = m_zPos + (time.GetTime()/1200) * dir;

	if ((m_zPos < -6) || (m_zPos > 50)) {
		 dir = dir * -1;
		 time.GetTime();
	}

	float m_fPitch = 0;
	
	float m_fRoll = 0;
	float m_fYaw = 0;

	m_fYaw  = 3.14 * (time2.PeekTime()/2000);
 
    // Using the left-to-right order of matrix concatenation,
    // apply the translation to the object's world position
    // before applying the rotations.
    D3DXMatrixTranslation(pMatWorld, m_xPos, m_yPos, m_zPos);
    D3DXMatrixIdentity(&MatRot);

    // Now, apply the orientation variables to the world matrix
	if ((x == 0)
		|| (x == 1) || (x == 2)) 
	{
    if(m_fPitch || m_fYaw || m_fRoll) {
        // Produce and combine the rotation matrices.
        D3DXMatrixRotationX(&MatTemp, m_fPitch);         // Pitch
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationY(&MatTemp, m_fYaw);           // Yaw
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationZ(&MatTemp, m_fRoll);          // Roll
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
 
        // Apply the rotation matrices to complete the world matrix.
        D3DXMatrixMultiply(pMatWorld, &MatRot, pMatWorld);
    }
	}

	if ((x==2) || (x==1)) {
		m_fRoll = (3.14/2) * (time2.PeekTime()/19000);
        D3DXMatrixRotationX(&MatTemp, m_fRoll);          // Roll
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        // Apply the rotation matrices to complete the world matrix.
//        D3DXMatrixMultiply(pMatWorld, &MatRot, pMatWorld);

	}
	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_WORLD, pMatWorld );
	//for (int x = 0; x++ ; x<1)
//   g_D3DObject->m_d3ddevice9->SetTextureStageState(0,D3DTSS_CONSTANT ,0);
}


void g_MainGameLoop() 
{
   static int tm = 1;
   static char debg[255];

   time.UpdateClock();
   if (--tm == 0) {
     sprintf (debg,"FPS %.2f\n",4000 / time.GetTime());
     OutputDebugString(debg);
	 tm = 4;
   }


   
   g_D3DObject->BeginPaint();
   g_ObjMgr.paint();
   g_D3DObject->DrawTextStr((int)(time2.PeekTime()/50)%800,100,D3DCOLOR_XRGB(240,0,50),"Hello World!!!");
   g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,0,50),debg);
   
   g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_AMBIENT,        0x00FFFFFF );
   D3DMATERIAL9 d3dMaterial;
   memset(&d3dMaterial, 0, sizeof(d3dMaterial));
   d3dMaterial.Diffuse.r = 1.0f;
   d3dMaterial.Diffuse.g = 1.0f;
   d3dMaterial.Diffuse.b = 1.0f;
   d3dMaterial.Diffuse.a = 1.0f;
   d3dMaterial.Ambient = d3dMaterial.Diffuse;
   d3dMaterial.Specular = d3dMaterial.Diffuse;
   d3dMaterial.Power = 100.0f; // arbitrary

   g_D3DObject->m_d3ddevice9->SetMaterial( &d3dMaterial );

    D3DXMATRIX matView;
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3( 0.0f, 0.0f, -10.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &matView, &vFromPt, &vLookatPt, &vUpVec );
    g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIX matProj;
    FLOAT fAspect = ((FLOAT)800) / 600;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );
	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_PROJECTION, &matProj );

	D3DXMATRIX matWorld;
	
   for (int x = 0; x < nMat; x++)  {
//	 g_D3DObject->m_d3ddevice9->SetMaterial( &tankmat[x] );
     MakeWorldMatrix(&matWorld,x);
	 g_D3DObject->m_d3ddevice9->SetTexture(0,tanktex[x]);
     //if (x == 1 || x == 2)
	 teapot->DrawSubset(x);
   }


   g_D3DObject->m_pd3dxSprite->Begin(0);
   tex1->Paint((int)(time2.PeekTime()/50)%800,(int)(time2.PeekTime()/50)%600);
   g_D3DObject->m_pd3dxSprite->End();

   g_D3DObject->EndPaint();
}
