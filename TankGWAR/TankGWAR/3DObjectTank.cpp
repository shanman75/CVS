#include "StdAfx.h"
#include "3DObject.h"
#include "3DObjectTank.h"
#include "3DObjectMissile.h"
#include "ObjMgr.h"
#include <stdio.h>

int					    c3DObjectTank::m_graph_init = 0;
LPD3DXMESH			c3DObjectTank::m_tankmesh = NULL;
DWORD				    c3DObjectTank::m_tankNmat = 0;
LPDIRECT3DTEXTURE9*	c3DObjectTank::m_tanktex = NULL;
D3DMATERIAL9*		c3DObjectTank::m_tankmat = NULL;


c3DObjectTank::c3DObjectTank()
{
	if (!m_graph_init++) _LoadGraphics();

  m_curmesh = m_tankmesh;
  m_nMat = m_tankNmat;
  m_curtex = m_tanktex;
  m_curmat = m_tankmat;

  m_barrelHeight = 0;
  m_turretRotate = 0;
//  m_orient = D3DXVECTOR3(1,0,0);
}

c3DObjectTank::~c3DObjectTank(void)
{
   if (!--m_graph_init) _UnloadGraphics();
}

void c3DObjectTank::MakeWorldMatrix( int x )
{
	D3DXMATRIX MatWorld; 
	D3DXMATRIX *pMatWorld = &MatWorld;
  D3DXMATRIX MatTemp;  // Temp matrix for rotations.
  D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
                         // pMatWorld.
	D3DXMATRIX MatRotY, MatTrY;   // Rotation Matrix for turret

  D3DXMatrixIdentity(&MatRot);
	D3DXMatrixIdentity(&MatRotY);

  D3DXMatrixTranslation(pMatWorld, m_position.x, m_position.y, m_position.z);

	if (x==4 || x==5) {
		D3DXVECTOR3 VecAxisY (1,0,0);  // Axis of Rotation

    D3DXMatrixTranslation(&MatTrY, 0, 0, -0.769);
		D3DXMatrixRotationAxis(&MatRotY,&VecAxisY,-m_barrelHeight);
		D3DXMatrixMultiply(&MatRotY,&MatTrY,&MatRotY);
    D3DXMatrixTranslation(&MatTrY, 0, 0, 0.739);
		D3DXMatrixMultiply(&MatRotY,&MatRotY,&MatTrY);
	}

	// Now, apply the orientation variables to the world matrix
	if ((x == 0) || (x==3) || (x==4) || (x==5)) 
	{
    D3DXMatrixRotationY(&MatTemp, m_turretRotate);
    D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	}

   if(m_orient.x || m_orient.z || m_orient.y) {
        // Produce and combine the rotation matrices.
        D3DXMatrixRotationX(&MatTemp, m_orient.x);         // Pitch
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationY(&MatTemp, m_orient.y);         // Pitch
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationZ(&MatTemp, m_orient.z);           // Yaw
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
    }

	D3DXMatrixMultiply(pMatWorld, &MatRotY, D3DXMatrixMultiply(&MatTemp,&MatRot,&MatWorld));

	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_WORLD, pMatWorld );
//  g_D3DObject->m_d3ddevice9->SetTextureStageState(0,D3DTSS_CONSTANT ,0);
}

void c3DObjectTank::paint()
{	
   for (int x = 0; x < (int)m_nMat; x++)  {
//	 g_D3DObject->m_d3ddevice9->SetMaterial( &tankmat[x] );
     MakeWorldMatrix(x);
	   g_D3DObject->m_d3ddevice9->SetTexture(0,m_tanktex[x]);
     //if (x == 1 || x == 2)
	   m_tankmesh->DrawSubset(x);
   }
}
void c3DObjectTank::_UnloadGraphics()
{
  m_tankmesh->Release();
  for (int x = 0; x < (int)m_nMat; x++) {
	  SAFE_RELEASE(m_tanktex[x]);
  }
  SAFE_DELETE_ARRAY(m_tanktex);
  SAFE_DELETE_ARRAY(m_tankmat);
}

void c3DObjectTank::event(enum EVENT evnt)
{
  switch (evnt) {
    case UP:
      m_barrelHeight -= 0.1; break;
    case DOWN:
      m_barrelHeight +=0.1; break;
    case LEFT:
      m_turretRotate-=0.1; break;
    case RIGHT:
      m_turretRotate+=0.1; break;
    default:
      break;
  }
  if (m_barrelHeight < 0) m_barrelHeight = 0;
  if (m_barrelHeight > D3DX_PI/2) m_barrelHeight = D3DX_PI/2;

  if (m_turretRotate < 0) m_turretRotate += 2*D3DX_PI;
  if (m_turretRotate > 2*D3DX_PI) m_turretRotate -= 2*D3DX_PI;
}
void c3DObjectTank::_LoadGraphics()
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
     &m_tankNmat,				// DWORD *pNumMaterials,
     &m_tankmesh
   )))
     m_tankmesh = NULL;
   else
     if( FAILED( hr = m_tankmesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
    {
        SAFE_RELEASE( pAdjacencyBuffer );
        SAFE_RELEASE( lpMat );
        return;
    }

   D3DXMATERIAL* mat = (D3DXMATERIAL*) lpMat->GetBufferPointer();
   char texpath[512];

   m_tanktex = new LPDIRECT3DTEXTURE9 [m_tankNmat];
   m_tankmat = new D3DMATERIAL9 [m_tankNmat];
 
   for (DWORD x = 0; x < m_tankNmat; x++) {
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

void c3DObjectTank::Fire(enum FIRE_TYPE fire)
{
  c3DObject *objadd = NULL;
  float m_power = 100;
  float xzsc    = 4.8;

  D3DXVECTOR3 tOrient = m_orient;
  tOrient.z   += m_turretRotate;
  tOrient.x   -= m_barrelHeight;

  D3DXVECTOR3 tTemp = tOrient;
  D3DXVECTOR3 tPosition = m_position + 1
    * D3DXVECTOR3(xzsc*sin(m_turretRotate),
     3.6*sin(m_barrelHeight),
     xzsc*cos(m_turretRotate));
  
  objadd = new c3DObjectMissile();
  objadd->accel   (D3DXVECTOR3(0,-12.8,0));
  objadd->velocity(D3DXVECTOR3(m_power/5*sin(m_turretRotate),
                               m_power/5*sin(m_barrelHeight),
                               m_power/5*cos(m_turretRotate)));
  objadd->orient  (tOrient);
  objadd->pos     (tPosition);
  g_ObjMgr->add(objadd);
}
