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
  m_firePower = 20.0f;

  m_barrelHeight = 0;
  m_turretRotate = 0;

  m_keytime.Reset();
  m_keytime.setInterval(20.0f);
  m_firetime.Reset();
  m_firetime.setInterval(600.0f);
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
  if (m_keytime.CmpTime())
  switch (evnt) {
    case UP:
      m_barrelHeight -= 0.1; break;
    case DOWN:
      m_barrelHeight +=0.1; break;
    case LEFT:
      m_turretRotate-=0.1; break;
    case RIGHT:
      m_turretRotate+=0.1; break;
    case PWRUP:
      m_firePower+=0.2f; break;
    case PWRDN:
      m_firePower-=0.2f; break;
    default:
      break;
  }
  if (m_barrelHeight < 0) m_barrelHeight = 0;
  if (m_barrelHeight > D3DX_PI/2) m_barrelHeight = D3DX_PI/2;

  if (m_turretRotate < 0) m_turretRotate += 2*D3DX_PI;
  if (m_turretRotate > 2*D3DX_PI) m_turretRotate -= 2*D3DX_PI;

  if (m_firePower < 1.2f) m_firePower = 1.2f;
  if (m_firePower > 250/5) m_firePower = 250/5;
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
  if (!m_firetime.CmpTime()) return;
  c3DObject *objadd = NULL;
  float turret_radius    = 0.8f;
  float barrel_length    = 2.7f;
  float missile_length   = 4.2f;


  D3DXVECTOR3 tOrient = m_orient;
  tOrient.z   += m_turretRotate;
  tOrient.x   -= m_barrelHeight;

  D3DXVECTOR3 tTemp = tOrient;
  D3DXVECTOR3 tPosition;
  D3DXVECTOR3 tVelocity = D3DXVECTOR3(sin(m_turretRotate)*cos(m_barrelHeight),
                               sin(m_barrelHeight),
                               cos(m_turretRotate)*cos(m_barrelHeight));
  
  // Calculate vector to turret
  tPosition = D3DXVECTOR3(
            turret_radius*sin(m_turretRotate),
            0,
            turret_radius*cos(m_turretRotate)
  );
  // Add in vector for barrel and 1/2 length of missile
  tPosition += tVelocity * barrel_length * missile_length / 2;
  tPosition += m_position;

  char debg[255];
  sprintf(debg,"x,z = (%.2f,%.2f)\n",tPosition.x,tPosition.z);
  OutputDebugString(debg);

  objadd = new c3DObjectMissile();
  objadd->accel   (D3DXVECTOR3(0,-12.8,0));
  objadd->velocity(tVelocity * m_firePower);
  objadd->orient  (tOrient);
  objadd->pos     (tPosition);
  g_ObjMgr->add(objadd);
}
