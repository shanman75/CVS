#include "StdAfx.h"
#include "3DObject.h"
#include <stdio.h>

c3DObject::c3DObject(D3DXVECTOR3 pos)
{
   m_position	=	pos;
   m_accel		=	D3DXVECTOR3(0,0,0);
   m_velocity	=	D3DXVECTOR3(0,0,0);
   m_orient		=	D3DXVECTOR3(0,0,0);
   m_scale    = D3DXVECTOR3(1,1,1);
   m_initVelocity = m_velocity;
   m_initOrient   = m_orient;

   m_time.Reset();
}

void c3DObject::pos     (D3DXVECTOR3 in) {   m_position = in; }
void c3DObject::accel   (D3DXVECTOR3 in) {   m_accel = in; }
void c3DObject::velocity(D3DXVECTOR3 in) {   m_velocity = in; m_initVelocity = in; }
void c3DObject::orient  (D3DXVECTOR3 in) {   m_orient   = in; m_initOrient   = in; }

c3DObject::~c3DObject(void)
{
}

void c3DObject::MakeWorldMatrix( int x )
{
	D3DXMATRIXA16 MatWorld; 
	D3DXMATRIXA16 *pMatWorld = &MatWorld;
 
  D3DXMATRIXA16 MatTemp;  // Temp matrix for rotations.
  D3DXMATRIXA16 MatRot;   // Final rotation matrix, applied to 
                         // pMatWorld.
	D3DXMATRIXA16 MatRotY, MatTrY;   // Rotation Matrix for turret		

	float m_fPitch = m_orient.x;
	float m_fRoll  = m_orient.y;
	float m_fYaw   = m_orient.z;

 
    // Using the left-to-right order of matrix concatenation,
    // apply the translation to the object's world position
    // before applying the rotations.
	
   D3DXMatrixIdentity(&MatRot);
	 D3DXMatrixIdentity(&MatRotY);

   D3DXMatrixTranslation(pMatWorld, m_position.x, m_position.y, m_position.z);

	// Now, apply the orientation variables to the world matrix
    if(m_fPitch || m_fYaw || m_fRoll) {
        // Produce and combine the rotation matrices.
        D3DXMatrixRotationX(&MatTemp, m_fPitch);         // Pitch
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationY(&MatTemp, m_fYaw);           // Yaw
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationZ(&MatTemp, m_fRoll);          // Roll
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
    }

//	D3DXMatrixMultiply(pMatWorld, &MatRotY, D3DXMatrixMultiply(&MatTemp,&MatRot,&MatWorld));
  D3DXMatrixMultiply(pMatWorld, D3DXMatrixScaling(&MatTemp, m_scale.x,m_scale.y,m_scale.z), &MatWorld);
	D3DXMatrixMultiply(pMatWorld, &MatRot,&MatWorld);

	g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_WORLD, pMatWorld );
}


void c3DObject::paint()
{
   D3DMATERIAL9 tmpMat;
   g_D3DObject->m_d3ddevice9->GetMaterial(&tmpMat);
   for (int x = 0; x < (int)m_nMat; x++)  {
     MakeWorldMatrix(x);
     if (m_curtex)
	     g_D3DObject->m_d3ddevice9->SetTexture(0,m_curtex[x]);
     g_D3DObject->m_d3ddevice9->SetMaterial(&m_curmat[x]);
	   m_curmesh->DrawSubset(x);
   }
   g_D3DObject->m_d3ddevice9->SetMaterial(&tmpMat);
}

void c3DObject::move()
{
	float tm = m_time.GetTime()/600;

	// Update velocity
	m_velocity += m_accel * tm;

	// Update position
	m_position += m_velocity * tm;

  /*
  char debg[366];
	sprintf(debg,"Moving object tm = %f, acc=%f, vel=%f, pos=%f\n",tm,
    m_accel.y,
    m_velocity.y,
    m_position);
  OutputDebugString(debg);
  */
}
