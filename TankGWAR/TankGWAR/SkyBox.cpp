#include "StdAfx.h"
#include "skybox.h"
#include <stdio.h>

LPDIRECT3DTEXTURE9*	  cSkyBox::m_tertex;
ID3DXMesh *cSkyBox::m_SkyMesh;

LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbnegx;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbposx;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbnegz;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbposz;
LPDIRECT3DVERTEXBUFFER9 cSkyBox::m_sbtop;

const DWORD BILLBOARDVERTEX::FVF=D3DFVF_XYZ|D3DFVF_TEX1; //flexible vertex format


cSkyBox::cSkyBox(void)
{
	m_w = SKY_WIDTH;
	m_h = SKY_HEIGHT;
  	_Init();
}

cSkyBox::~cSkyBox(void)
{
  m_tertex[0]->Release();
  if (m_SkyMesh) m_SkyMesh->Release();
  m_sbtop->Release();
  m_sbposz->Release();
  m_sbnegz->Release();
  m_sbnegx->Release();
  m_sbposx->Release();
}

void cSkyBox::Paint()
{
    D3DXMATRIXA16 matView, matViewSave;
    //g_D3DObject->m_d3ddevice9->GetTransform( D3DTS_VIEW, &matViewSave );
    //matView = matViewSave;
    //matView._41 = 0; matView._42 = 0.0f; matView._43 = 0;
    //g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW,      &matView );
    g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZENABLE, FALSE );
    g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );


	D3DXMATRIX matwrld;
	D3DXMatrixIdentity(&matwrld);
	g_D3DObject->m_d3ddevice9->SetTransform(D3DTS_WORLD,&matwrld);
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = 1.0f;
    mtrl.Diffuse.g = 1.0f;
    mtrl.Diffuse.b = 1.0f;
    mtrl.Diffuse.a = 1.0f;
    mtrl.Specular = mtrl.Diffuse;
    mtrl.Ambient = mtrl.Diffuse;
    mtrl.Power = 100.0f;
    g_D3DObject->m_d3ddevice9->SetMaterial( &mtrl );
	if (m_sbnegx!= NULL)
  {    
      g_D3DObject->m_d3ddevice9->SetFVF(BILLBOARDVERTEX::FVF); 

	  g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource(0,m_sbnegx,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource(0,m_sbposx,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource(0,m_sbnegz,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[0]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource(0,m_sbposz,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	  g_D3DObject->m_d3ddevice9->SetTexture(0,m_tertex[1]);
	  g_D3DObject->m_d3ddevice9->SetStreamSource(0,m_sbtop,0,sizeof(BILLBOARDVERTEX));
	  g_D3DObject->m_d3ddevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
  }

  //g_D3DObject->m_d3ddevice9->SetTransform( D3DTS_VIEW,      &matViewSave );
  g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZENABLE, TRUE );
  g_D3DObject->m_d3ddevice9->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
}

BOOL cSkyBox::_Init()
{

  /*
  D3DXCreateMeshFVF( 2, 4 , 
	  D3DXMESH_32BIT | D3DXMESH_SYSTEMMEM, 
	  D3DFVF_SKYMESH, 
	  g_D3DObject->m_d3ddevice9, 
	  &m_SkyMesh 
	);
  */

  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				BILLBOARDVERTEX::FVF,
				D3DPOOL_MANAGED,
				&m_sbnegx,
				NULL))
	)
        return FALSE; //bail if failed

  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				BILLBOARDVERTEX::FVF,
				D3DPOOL_MANAGED,
				&m_sbposx,
				NULL))
	)
        return FALSE; //bail if failed

    if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				BILLBOARDVERTEX::FVF,
				D3DPOOL_MANAGED,
				&m_sbnegz,
				NULL))
	)
        return FALSE; //bail if failed


  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				BILLBOARDVERTEX::FVF,
				D3DPOOL_MANAGED,
				&m_sbposz,
				NULL))
	)
        return FALSE; //bail if failed


  if (FAILED(g_D3DObject->m_d3ddevice9->CreateVertexBuffer( 
				4*sizeof(BILLBOARDVERTEX),
				D3DUSAGE_WRITEONLY,
				BILLBOARDVERTEX::FVF,
				D3DPOOL_MANAGED,
				&m_sbtop,
				NULL))
	)
        return FALSE; //bail if failed

  
  float h = m_h;
  float w = m_w;

  BILLBOARDVERTEX *pTetVB = NULL; //Pointer to vertex buffer.

  if(SUCCEEDED(m_sbnegx->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
     pTetVB->p=D3DXVECTOR3 (-w/2,h,-w/2);     pTetVB->tu=1.0f;  	pTetVB->tv=0.0f;
  (++pTetVB)->p=D3DXVECTOR3(-w/2,h,w/2);   pTetVB->tu=0.0f;     pTetVB->tv=0.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,0,-w/2);    pTetVB->tu=1.0f;		pTetVB->tv=1.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,0,w/2);   pTetVB->tu=0.0f;		pTetVB->tv=1.0;
    m_sbnegx->Unlock();
  }

  if(SUCCEEDED(m_sbposx->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3(w/2,h,w/2);    pTetVB->tu=1.0f;  	pTetVB->tv=0.0f;
  (++pTetVB)->p=D3DXVECTOR3(w/2,h,-w/2);    pTetVB->tu=0.0f;    pTetVB->tv=0.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(w/2,0,w/2);    pTetVB->tu=1.0f;	pTetVB->tv=1.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(w/2,0,-w/2);	 pTetVB->tu=0.0f;	pTetVB->tv=1.0;
    m_sbposx->Unlock();
  }

  if(SUCCEEDED(m_sbnegz->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3(w/2,h,-w/2);    pTetVB->tu=1.0f;  	pTetVB->tv=0.0f;
  (++pTetVB)->p=D3DXVECTOR3(-w/2,h,-w/2);    pTetVB->tu=0.0f;    pTetVB->tv=0.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(w/2,0,-w/2);    pTetVB->tu=1.0f;	pTetVB->tv=1.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,0,-w/2);	 pTetVB->tu=0.0f;	pTetVB->tv=1.0;
    m_sbnegz->Unlock();
  }

  if(SUCCEEDED(m_sbposz->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3(-w/2,h, w/2);    pTetVB->tu=1.0f;  	pTetVB->tv=0.0f;
  (++pTetVB)->p=D3DXVECTOR3(w/2,h, w/2);    pTetVB->tu=0.0f;    pTetVB->tv=0.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,0, w/2);    pTetVB->tu=1.0f;	pTetVB->tv=1.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(w/2,0,w/2);	 pTetVB->tu=0.0f;	pTetVB->tv=1.0;
    m_sbposz->Unlock();
  }

  if(SUCCEEDED(m_sbtop->Lock(0,0,(void**)&pTetVB,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    pTetVB->p=D3DXVECTOR3(w/2,h,w/2);    pTetVB->tu=1.0f;  	pTetVB->tv=0.0f;
  (++pTetVB)->p=D3DXVECTOR3(-w/2,h,w/2);    pTetVB->tu=0.0f;    pTetVB->tv=0.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(w/2,h,-w/2);    pTetVB->tu=1.0f;	pTetVB->tv=1.0f;
    
  (++pTetVB)->p=D3DXVECTOR3(-w/2,h,-w/2);	 pTetVB->tu=0.0f;	pTetVB->tv=1.0;
    m_sbtop->Unlock();
  }


  char texpath[1024];
  LPDIRECT3DTEXTURE9 tempt;

  m_tertex = new LPDIRECT3DTEXTURE9 [2];
  sprintf (texpath,"resource\\%s","DSC05370.JPG");
  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
    return false;
  m_tertex[0] = tempt;

  sprintf (texpath,"resource\\%s","skytop.jpg");
  if (D3DXCreateTextureFromFile(g_D3DObject->m_d3ddevice9, texpath, &tempt)!= D3D_OK)
    return false;
  m_tertex[1] = tempt;

	sSkyVertex *VertexPtr;
	DWORD *IndexPtr;

	IndexPtr =NULL;
	VertexPtr =NULL;

	//m_SkyMesh->LockVertexBuffer( 0, (LPVOID *) &VertexPtr );	
	//m_SkyMesh->LockIndexBuffer( 0, (LPVOID *) &IndexPtr );	
    
	/*
    VertexPtr[0].p=D3DXVECTOR3(0,m_h,0); 
    VertexPtr[0].tu=1.0f; VertexPtr[0].tv=0.0f;

    VertexPtr[1].p=D3DXVECTOR3(0,m_h,-m_w);
    VertexPtr[1].tu=0.0f; VertexPtr[1].tv=0.0f;
    
    VertexPtr[2].p=D3DXVECTOR3(0,0,0);
    VertexPtr[2].tu=1.0f; VertexPtr[2].tv=1.0f;
    
    VertexPtr[3].p=D3DXVECTOR3(0,0,-m_w);
    VertexPtr[3].tu=	0.0f; VertexPtr[3].tv=1.0;


        posx[0].p=D3DXVECTOR3(w,h,-w);
    posx[0].tu=1.0f; posx[0].tv=0.0f;
    posx[1].p=D3DXVECTOR3(w,h,0);
    posx[1].tu=0.0f; posx[1].tv=0.0f;
    posx[2].p=D3DXVECTOR3(w,0,-w);
    posx[2].tu=1.0f; posx[2].tv=1.0f;
    posx[3].p=D3DXVECTOR3(w,0,0);
    posx[3].tu=0.0f; posx[3].tv=1.0f;

    */

// Draw the four sides
   //*IndexPtr++ = 0;   *IndexPtr++ = 1;    *IndexPtr++ = 2;
   //*IndexPtr++ = 3;   *IndexPtr++ = 2;    *IndexPtr++ = 1;

   /*
   *IndexPtr++ = 1;   *IndexPtr++ = 5;    *IndexPtr++ = 3;
   *IndexPtr++ = 7;   *IndexPtr++ = 3;    *IndexPtr++ = 5;

   *IndexPtr++ = 3;   *IndexPtr++ = 7;    *IndexPtr++ = 2;
   *IndexPtr++ = 6;   *IndexPtr++ = 2;    *IndexPtr++ = 7;

   *IndexPtr++ = 2;   *IndexPtr++ = 6;    *IndexPtr++ = 0;
   *IndexPtr++ = 4;   *IndexPtr++ = 0;    *IndexPtr++ = 6;

// Then the top
   *IndexPtr++ = 5;  *IndexPtr++ = 4;     *IndexPtr++ = 7;
   *IndexPtr++ = 6;  *IndexPtr++ = 7;     *IndexPtr++ = 4;
   */

//	m_SkyMesh->UnlockIndexBuffer();
//  m_SkyMesh->UnlockVertexBuffer();	

	/*
  m_SkyMesh->OptimizeInplace(
                            D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE,
                            NULL, NULL, NULL, NULL);
    */
	//D3DXComputeNormals(g_SkyMesh,NULL);
	return true;
}