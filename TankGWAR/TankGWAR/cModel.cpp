#include "StdAfx.h"
#include "cmodel.h"
#include <stdio.h>

int					cModel::m_graph_init = 0;
LPD3DXMESH			cModel::m_tank = NULL;
DWORD				cModel::m_nMat = 0;
LPDIRECT3DTEXTURE9*	cModel::m_tanktex = NULL;
D3DMATERIAL9*		cModel::m_tankmat = NULL;


cModel::cModel(float x, float y, float z)
{
	m_xPos = x;
	m_yPos = y;
	m_zPos = z;

	if (!m_graph_init++) _LoadGraphics();
}

cModel::~cModel(void)
{
   if (!--m_graph_init) _UnloadGraphics();
}

void cModel::MakeWorldMatrix( int x )
{
	D3DXMATRIXA16 MatWorld; 
	D3DXMATRIXA16 *pMatWorld = &MatWorld;
  D3DXMATRIXA16 MatTemp;  // Temp matrix for rotations.
  D3DXMATRIXA16 MatRot;   // Final rotation matrix, applied to 
                         // pMatWorld.
	D3DXMATRIXA16 MatRotY, MatTrY;   // Rotation Matrix for turret

	static dir = -1;
	
	//m_zPos = m_zPos + (m_time.GetTime()/1200) * dir;

	if ((m_zPos < -6) || (m_zPos > 50)) {
		 dir = dir * -1;
		 m_time.GetTime();
	}

	float m_fPitch = 0;
	
	float m_fRoll = 0;
	float m_fYaw = 0;

	m_fYaw  = D3DX_PI * (m_time2.PeekTime()/2000);
	//m_fYaw = 3.14/2;
	//m_fYaw = 0;
 
    // Using the left-to-right order of matrix concatenation,
    // apply the translation to the object's world position
    // before applying the rotations.
	
    D3DXMatrixIdentity(&MatRot);
	D3DXMatrixIdentity(&MatRotY);

    D3DXMatrixTranslation(pMatWorld, m_xPos, m_yPos, m_zPos);

	if (x==4 || x==5) {
			D3DXVECTOR3 VecAxisY (1,0,0);  // Axis of Rotation

		float m_barrelHeight = fmod((D3DX_PI/2) * (m_time2.PeekTime()/12000),D3DX_PI/2);

//		char ch[255];
//		sprintf (ch,"Barrel Height = %f cos=%f\n", m_barrelHeight, cos(m_barrelHeight));
//		OutputDebugString (ch);
		
		//m_fRoll = 3.14/4;
        //D3DXMatrixRotationX(&MatRotY, -m_barrelHeight);          // Roll

    D3DXMatrixTranslation(&MatTrY, 0.0f, 0.0f, -0.769f);
		D3DXMatrixRotationAxis(&MatRotY,&VecAxisY,-m_barrelHeight);
		D3DXMatrixMultiply(&MatRotY,&MatTrY,&MatRotY);
    D3DXMatrixTranslation(&MatTrY, 0.0f, 0.0f, 0.739f);
		D3DXMatrixMultiply(&MatRotY,&MatRotY,&MatTrY);
		

		/*
		MatRotY._11 = MatRotY._44  = 1;
		MatRotY._12 = MatRotY._13 = MatRotY._14 = MatRotY._21 =
			MatRotY._24 = MatRotY._31 = MatRotY._34 =
			MatRotY._41 = MatRotY._42 = MatRotY._43 = 0;
        MatRotY._22 = cos (m_barrelHeight);
		MatRotY._23 = -sin(m_barrelHeight);
		MatRotY._32 = sin (m_barrelHeight);
		MatRotY._33 = cos (m_barrelHeight);
		*/
        //D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);

		//pMatWorld->_41 += (5.0f) * sin(m_barrelHeight);
		//pMatWorld->_42 += (0.4f) * (1-cos(m_barrelHeight));
		//pMatWorld->_43 += (3.3f) * sin(m_barrelHeight);
	}

	// Now, apply the orientation variables to the world matrix
	if ((x == 0) || (x==3) || (x==4) || (x==5)) 
	{
    if(m_fPitch || m_fYaw || m_fRoll) {
        // Produce and combine the rotation matrices.
        D3DXMatrixRotationX(&MatTemp, m_fPitch);         // Pitch
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationY(&MatTemp, m_fYaw);           // Yaw
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
//        D3DXMatrixRotationZ(&MatTemp, m_fRoll);          // Roll
//        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
 
        // Apply the rotation matrices to complete the world matrix.
    }
	}

	//D3DXMatrixMultiply(pMatWorld, &MatRot, pMatWorld);
	D3DXMatrixMultiply(pMatWorld, &MatRotY, D3DXMatrixMultiply(&MatTemp,&MatRot,&MatWorld));

	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_WORLD, pMatWorld );
	for (int x = 0; x<1 ;x++)
      g_D3DObject->m_d3ddevice9->SetTextureStageState(0,D3DTSS_CONSTANT ,0);
}

void cModel::Paint()
{	
   for (int x = 0; x < (int)m_nMat; x++)  {
//	 g_D3DObject->m_d3ddevice9->SetMaterial( &tankmat[x] );
     MakeWorldMatrix(x);
	 g_D3DObject->m_d3ddevice9->SetTexture(0,m_tanktex[x]);
     //if (x == 1 || x == 2)
	 m_tank->DrawSubset(x);
   }
}
void cModel::_UnloadGraphics()
{
  m_tank->Release();
  for (int x = 0; x < (int)m_nMat; x++) {
	  SAFE_RELEASE(m_tanktex[x]);
  }
  SAFE_DELETE_ARRAY(m_tanktex);
  SAFE_DELETE_ARRAY(m_tankmat);
}
void cModel::_LoadGraphics()
{
	LPD3DXBUFFER lpMat = NULL;
    LPD3DXBUFFER pAdjacencyBuffer = NULL;
	HRESULT hr;

  if (FAILED(D3DXLoadMeshFromX(
     "resource\\tank2.x",
     D3DXMESH_SYSTEMMEM,
	 g_D3DObject->m_d3ddevice9,
     &pAdjacencyBuffer,				// LPD3DXBUFFER *ppAdjacency,
     &lpMat,				// LPD3DXBUFFER *ppMaterials,
     NULL,			    // LPD3DXBUFFER *ppEffectInstances,
     &m_nMat,				// DWORD *pNumMaterials,
     &m_tank
   )))
     m_tank = NULL;
   else
     if( FAILED( hr = m_tank->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
    {
        SAFE_RELEASE( pAdjacencyBuffer );
        SAFE_RELEASE( lpMat );
        return;
    }

   D3DXMATERIAL* mat = (D3DXMATERIAL*) lpMat->GetBufferPointer();
   char texpath[512];

   m_tanktex = new LPDIRECT3DTEXTURE9 [m_nMat];
   m_tankmat = new D3DMATERIAL9 [m_nMat];
 
   for (DWORD x = 0; x < m_nMat; x++) {
	   LPDIRECT3DTEXTURE9 tempt;
	   tempt = NULL;
	   m_tankmat[x] = mat[x].MatD3D;
	   m_tanktex[x] = NULL;
	   if (mat[x].pTextureFilename != NULL) {               
		        sprintf (texpath,"resource\\%s",mat[x].pTextureFilename);
	       if (hr = D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
		   {
			   m_tanktex[x] = NULL;
			   OutputDebugString("Create Texture Failed\n");
		   }
		   else {
			   m_tanktex[x] = tempt;
		   }
	   }
//      g_D3DObject->LoadTextureFromFile(texpath,&tanktex[nMat],0);
   }
    SAFE_RELEASE( pAdjacencyBuffer );
    SAFE_RELEASE( lpMat );
}