#include "StdAfx.h"
#include "3dobjectmissile.h"
#include <stdio.h>

LPD3DXMESH			c3DObjectMissile::m_missilemesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_missiletex;
D3DMATERIAL9*		c3DObjectMissile::m_missilemat;
DWORD				c3DObjectMissile::m_missileNmat;
//enum MSLTYPE { SHELL, ATOMBOMB, SCUD, AMRAM, FUNKIEBOMB };

// SHELL
LPD3DXMESH			    c3DObjectMissile::m_SHELL_mesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_SHELL_tex;
D3DMATERIAL9*		    c3DObjectMissile::m_SHELL_mat;
DWORD				        c3DObjectMissile::m_SHELL_Nmat;

// ATOMBOMB
LPD3DXMESH			    c3DObjectMissile::m_ATOMBOMB_mesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_ATOMBOMB_tex;
D3DMATERIAL9*		    c3DObjectMissile::m_ATOMBOMB_mat;
DWORD				        c3DObjectMissile::m_ATOMBOMB_Nmat;

// SCUD
LPD3DXMESH			    c3DObjectMissile::m_SCUD_mesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_SCUD_tex;
D3DMATERIAL9*		    c3DObjectMissile::m_SCUD_mat;
DWORD				        c3DObjectMissile::m_SCUD_Nmat;

// AMRAM
LPD3DXMESH			    c3DObjectMissile::m_AMRAM_mesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_AMRAM_tex;
D3DMATERIAL9*		    c3DObjectMissile::m_AMRAM_mat;
DWORD				        c3DObjectMissile::m_AMRAM_Nmat;

// FUNKIEBOMB
LPD3DXMESH			    c3DObjectMissile::m_FUNKIEBOMB_mesh;
LPDIRECT3DTEXTURE9*	c3DObjectMissile::m_FUNKIEBOMB_tex;
D3DMATERIAL9*		    c3DObjectMissile::m_FUNKIEBOMB_mat;
DWORD				        c3DObjectMissile::m_FUNKIEBOMB_Nmat;

int c3DObjectMissile::m_graph_init = 0;

float clamp (float z, float q)
{
  if (z > q) return q;
  else if (-z > q) return q;
  else return z;
}

void c3DObjectMissile::move()
{
  //m_orient.x = sin(m_velocity.y/m_initYvelocity);
  //m_orient.z = cos(m_velocity.y/m_initYvelocity);
  //m_orient.x = -sin(m_velocity.y/m_initYvelocity) * D3DX_PI/2;
  //if (m_velocity.y < 0) m_orient.x = clamp(m_orient.x+m_time.PeekTime()/(120*(m_velocity.x+m_velocity.z)),D3DX_PI/2);

  float xzLen = 0;
  xzLen = sqrt(m_velocity.x*m_velocity.x + m_velocity.z*m_velocity.z);
  if (xzLen == 0) { xzLen = 0.001f; }
  
  m_orient.x = clamp(-atan(m_velocity.y/xzLen),D3DX_PI/2);
//  m_orient.y = -atan(m_velocity.y/10);
  c3DObject::move();
}

c3DObjectMissile::c3DObjectMissile(MSLTYPE missile_type)
{
	if (!m_graph_init++) _LoadGraphics();

  switch (missile_type) {
    case AMRAM:
      m_curmesh =  m_AMRAM_mesh;
      m_nMat    =  m_AMRAM_Nmat;
      m_curtex  =  m_AMRAM_tex;
      m_curmat  =  m_AMRAM_mat;
      break;
    case FUNKIEBOMB:
      m_curmesh =  m_FUNKIEBOMB_mesh;
      m_nMat    =  m_FUNKIEBOMB_Nmat;
      m_curtex  =  m_FUNKIEBOMB_tex;
      m_curmat  =  m_FUNKIEBOMB_mat;
      break;
    case SHELL:
      m_curmesh =  m_SHELL_mesh;
      m_nMat    =  m_SHELL_Nmat;
      m_curtex  =  m_SHELL_tex;
      m_curmat  =  m_SHELL_mat;
      break;
    case ATOMBOMB:
      m_curmesh =  m_ATOMBOMB_mesh;
      m_nMat    =  m_ATOMBOMB_Nmat;
      m_curtex  =  m_ATOMBOMB_tex;
      m_curmat  =  m_ATOMBOMB_mat;
      break;
    case SCUD:
      m_curmesh =  m_SCUD_mesh;
      m_nMat    =  m_SCUD_Nmat;
      m_curtex  =  m_SCUD_tex;
      m_curmat  =  m_SCUD_mat;
      break;
    default:
      m_curmesh =  m_ATOMBOMB_mesh;
      m_nMat    =  m_ATOMBOMB_Nmat;
      m_curtex  =  m_ATOMBOMB_tex;
      m_curmat  =  m_ATOMBOMB_mat;
    break;
  }
  m_time.Reset();
  m_initYvelocity=-999.0f;

//  m_velocity = D3DXVECTOR3((rand()%80/10)-4,9,5);
//  m_accel    = D3DXVECTOR3(0,-3,0);

}

c3DObjectMissile::~c3DObjectMissile(void)
{
	if (!--m_graph_init) _UnloadGraphics();
}


void c3DObjectMissile::_UnloadGraphics()
{
  SAFE_RELEASE(m_ATOMBOMB_mesh);
  for (int x = 0; x < (int)m_nMat; x++) {
	  SAFE_RELEASE(m_ATOMBOMB_tex[x]);
  }
  SAFE_DELETE_ARRAY(m_ATOMBOMB_tex);
  SAFE_DELETE_ARRAY(m_ATOMBOMB_mat);
}
void c3DObjectMissile::_LoadGraphics()
{
	LPD3DXBUFFER  t_lpMat = NULL;
  LPD3DXBUFFER  t_pAdjacencyBuffer = NULL;
  D3DXMATERIAL* t_mat;
  char texpath[2048];

  // Atom Bomb
  if (FAILED(D3DXLoadMeshFromX("resource\\missiles\\abomb.X",
        D3DXMESH_SYSTEMMEM, g_D3DObject->m_d3ddevice9, &t_pAdjacencyBuffer,
        &t_lpMat, NULL, &m_ATOMBOMB_Nmat, &m_ATOMBOMB_mesh)))      exit(1);
  if( FAILED( m_ATOMBOMB_mesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)t_pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
                        exit(0);

   t_mat = (D3DXMATERIAL*) t_lpMat->GetBufferPointer();

   m_ATOMBOMB_tex = new LPDIRECT3DTEXTURE9 [m_ATOMBOMB_Nmat];
   m_ATOMBOMB_mat = new D3DMATERIAL9 [m_ATOMBOMB_Nmat];
 
   for (DWORD x = 0; x < m_ATOMBOMB_Nmat; x++) {
	   LPDIRECT3DTEXTURE9 tempt = NULL;
     m_ATOMBOMB_mat[x] = t_mat[x].MatD3D;
     m_ATOMBOMB_tex[x] = NULL;
     if (t_mat[x].pTextureFilename != NULL) {               
		     sprintf (texpath,"resource\\missiles\\%s",t_mat[x].pTextureFilename);
	       if (SUCCEEDED(D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)))
			       m_ATOMBOMB_tex[x] = tempt;
	   }     
   }
   SAFE_RELEASE( t_pAdjacencyBuffer ); SAFE_RELEASE( t_lpMat );

   // Scud
  if (FAILED(D3DXLoadMeshFromX("resource\\missiles\\scud.X",
        D3DXMESH_SYSTEMMEM, g_D3DObject->m_d3ddevice9, &t_pAdjacencyBuffer,
        &t_lpMat, NULL, &m_SCUD_Nmat, &m_SCUD_mesh)))      exit(1);
  if( FAILED( m_SCUD_mesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)t_pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
                        exit(0);

   t_mat = (D3DXMATERIAL*) t_lpMat->GetBufferPointer();

   m_SCUD_tex = new LPDIRECT3DTEXTURE9 [m_SCUD_Nmat];
   m_SCUD_mat = new D3DMATERIAL9 [m_SCUD_Nmat];
 
   for (DWORD x = 0; x < m_SCUD_Nmat; x++) {
	   LPDIRECT3DTEXTURE9 tempt = NULL;
     m_SCUD_mat[x] = t_mat[x].MatD3D;
     m_SCUD_tex[x] = NULL;
     if (t_mat[x].pTextureFilename != NULL) {               
		     sprintf (texpath,"resource\\missiles\\%s",t_mat[x].pTextureFilename);
	       if (SUCCEEDED(D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)))
			       m_SCUD_tex[x] = tempt;
	   }     
   }
   SAFE_RELEASE( t_pAdjacencyBuffer ); SAFE_RELEASE( t_lpMat );

   // Shell
  if (FAILED(D3DXLoadMeshFromX("resource\\missiles\\shell.X",
        D3DXMESH_SYSTEMMEM, g_D3DObject->m_d3ddevice9, &t_pAdjacencyBuffer,
        &t_lpMat, NULL, &m_SHELL_Nmat, &m_SHELL_mesh)))      exit(1);
  if( FAILED( m_SHELL_mesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)t_pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
                        exit(0);

   t_mat = (D3DXMATERIAL*) t_lpMat->GetBufferPointer();

   m_SHELL_tex = new LPDIRECT3DTEXTURE9 [m_SHELL_Nmat];
   m_SHELL_mat = new D3DMATERIAL9 [m_SHELL_Nmat];
 
   for (DWORD x = 0; x < m_SHELL_Nmat; x++) {
	   LPDIRECT3DTEXTURE9 tempt = NULL;
     m_SHELL_mat[x] = t_mat[x].MatD3D;
     m_SHELL_tex[x] = NULL;
     if (t_mat[x].pTextureFilename != NULL) {               
		     sprintf (texpath,"resource\\missiles\\%s",t_mat[x].pTextureFilename);
	       if (SUCCEEDED(D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)))
			       m_SHELL_tex[x] = tempt;
	   }     
   }
   SAFE_RELEASE( t_pAdjacencyBuffer ); SAFE_RELEASE( t_lpMat );

   // AM-RAM
  if (FAILED(D3DXLoadMeshFromX("resource\\missiles\\scud.x",
        D3DXMESH_SYSTEMMEM, g_D3DObject->m_d3ddevice9, &t_pAdjacencyBuffer,
        &t_lpMat, NULL, &m_AMRAM_Nmat, &m_AMRAM_mesh)))      exit(1);
  if( FAILED( m_AMRAM_mesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)t_pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
                        exit(0);

   t_mat = (D3DXMATERIAL*) t_lpMat->GetBufferPointer();

   m_AMRAM_tex = new LPDIRECT3DTEXTURE9 [m_AMRAM_Nmat];
   m_AMRAM_mat = new D3DMATERIAL9 [m_AMRAM_Nmat];
 
   for (DWORD x = 0; x < m_AMRAM_Nmat; x++) {
	   LPDIRECT3DTEXTURE9 tempt = NULL;
     m_AMRAM_mat[x] = t_mat[x].MatD3D;
     m_AMRAM_mat[x].Diffuse = D3DXCOLOR(0.1f,0.3f,1.0f,1.0f);
     m_AMRAM_mat[x].Ambient = D3DXCOLOR(0.1f,0.3f,1.0f,1.0f);
     m_AMRAM_tex[x] = NULL;
     if (t_mat[x].pTextureFilename != NULL) {               
		     sprintf (texpath,"resource\\missiles\\%s",t_mat[x].pTextureFilename);
	       if (SUCCEEDED(D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)))
			       m_AMRAM_tex[x] = tempt;
	   }     
   }
   SAFE_RELEASE( t_pAdjacencyBuffer ); SAFE_RELEASE( t_lpMat );

   // Funkie Bomb
  if (FAILED(D3DXLoadMeshFromX("resource\\missiles\\funkiebomb.x",
        D3DXMESH_SYSTEMMEM, g_D3DObject->m_d3ddevice9, &t_pAdjacencyBuffer,
        &t_lpMat, NULL, &m_FUNKIEBOMB_Nmat, &m_FUNKIEBOMB_mesh)))      exit(1);
  if( FAILED( m_FUNKIEBOMB_mesh->OptimizeInplace(
                        D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                        (DWORD*)t_pAdjacencyBuffer->GetBufferPointer(), NULL, NULL, NULL ) ) )
                        exit(0);

   t_mat = (D3DXMATERIAL*) t_lpMat->GetBufferPointer();

   m_FUNKIEBOMB_tex = new LPDIRECT3DTEXTURE9 [m_FUNKIEBOMB_Nmat];
   m_FUNKIEBOMB_mat = new D3DMATERIAL9 [m_FUNKIEBOMB_Nmat];
 
   for (DWORD x = 0; x < m_FUNKIEBOMB_Nmat; x++) {
	   LPDIRECT3DTEXTURE9 tempt = NULL;
     m_FUNKIEBOMB_mat[x] = t_mat[x].MatD3D;
     m_FUNKIEBOMB_tex[x] = NULL;
     if (t_mat[x].pTextureFilename != NULL) {               
		     sprintf (texpath,"resource\\missiles\\%s",t_mat[x].pTextureFilename);
	       if (SUCCEEDED(D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)))
			       m_FUNKIEBOMB_tex[x] = tempt;
	   }     
   }
   SAFE_RELEASE( t_pAdjacencyBuffer ); SAFE_RELEASE( t_lpMat );

}

float c3DObjectMissile::GetMissileLength(enum MSLTYPE m)
{
  switch (m)
  {
  case FUNKIEBOMB:
    return 10.0f; break;
  case AMRAM:
    return 10.0f; break;
  case ATOMBOMB:
    return 15.0f; break;
  case SHELL:
    return 10.0f; break;
  case SCUD:
    return 10.0f; break;
  default:
    return 10.0f; break;
  }
}

char *c3DObjectMissile::GetMissileStr(enum MSLTYPE m)
{
  //  enum MSLTYPE { SHELL, ATOMBOMB, SCUD, AMRAM, FUNKIEBOMB };

  switch (m)
  {
  case FUNKIEBOMB:
    return "F-Bomb"; break;
  case AMRAM:
    return "AM-RAM"; break;
  case ATOMBOMB:
    return "A-Bomb"; break;
  case SHELL:
    return "Shell"; break;
  case SCUD:
    return "Scud"; break;
  default:
    return "NONE"; break;
  }
}

D3DCOLORVALUE c3DObjectMissile::GetMissileExpColor(enum MSLTYPE m)
{
    switch (m)
  {
  case FUNKIEBOMB:
    return D3DXCOLOR(140.0f,140.0f,10.0f,0.9f); break;
  case AMRAM:
    return D3DXCOLOR(40.0f,255.0f,10.0f,0.75f); break;
  case ATOMBOMB:
    return D3DXCOLOR(255.0f,40.0f,90.0f,0.9f); break;
  case SHELL:
    return D3DXCOLOR(140.0f,140.0f,255.0f,0.7f); break;
  case SCUD:
    return D3DXCOLOR(40.0f,255.0f,30.0f,0.9f); break;
  default:
    return D3DXCOLOR(140.0f,140.0f,10.0f,1.0f); break;
  }
}

float c3DObjectMissile::GetMissileExpRadius(enum MSLTYPE m)
{
    switch (m)
  {
  case FUNKIEBOMB:
    return 35.0f; break;
  case AMRAM:
    return 40.0f; break;
  case ATOMBOMB:
    return 60.0f; break;
  case SHELL:
    return 7.0f; break;
  case SCUD:
    return 22.0f; break;
  default:
    return 15.0f; break;
  }
}

float c3DObjectMissile::GetMissileCost(enum MSLTYPE m)
{
    switch (m)
  {
  case FUNKIEBOMB:
    return 8.6f*1000000; break;
  case AMRAM:
    return 17.6f*1000000; break;
  case ATOMBOMB:
    return 24.6f*1000000; break;
  case SHELL:
    return 0.0f; break;
  case SCUD:
    return 2.6f*1000000; break;
  default:
    return 15.0f; break;
  }
}