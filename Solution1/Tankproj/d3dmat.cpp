/****************************************************************

d3dmat.cpp

File includes d3d setup functions that don't involve initializing
the graphics hardware.  Functions include setting up matrices
and lighting.

Last updated: April 8,2004
****************************************************************/

#include "d3dmat.h"
#include "initialization.h"

extern IDirect3D9 *d3dcomobj;
extern IDirect3DDevice9 *d3ddevice;
extern D3DPRESENT_PARAMETERS presparams;
//extern IDirect3DSurface9* backbuffer;
//extern IDirect3DSurface9* testscreen;
extern LPDIRECT3DTEXTURE9 BackgroundTexture; //texture for background
extern LPDIRECT3DTEXTURE9 skytexture; //texture for background
extern LPDIRECT3DVERTEXBUFFER9 sbnegx;
extern LPDIRECT3DVERTEXBUFFER9 sbposx;
extern LPDIRECT3DVERTEXBUFFER9 sbnegz;
extern LPDIRECT3DVERTEXBUFFER9 sbposz;
extern LPDIRECT3DVERTEXBUFFER9 sbtop;
extern CInputManager* InputManager; //input manager

void SetWorldMatrix(float dx=0.0f)
{ //set world matrix
  D3DXMATRIXA16 matWorld;    
  D3DXMatrixTranslation(&matWorld,dx,0.0,0.0);
  d3ddevice->SetTransform(D3DTS_WORLD,&matWorld );
} //SetWorldMatrix

void SetViewMatrix(float x=0,float y=0,bool cameraontanks=true)
{ //set view matrix
  D3DXVECTOR3 vUpVec(0.0f,1.0f,0.0f); //up vector
  D3DXVECTOR3 vEyePt; //camera position
  D3DXVECTOR3 vLookatPt; //camera view vector
  D3DXMATRIXA16 matView; //view matrix
  vEyePt=D3DXVECTOR3(0,333,1000);
  vLookatPt=D3DXVECTOR3(640,480,-640);
  D3DXMatrixLookAtRH(&matView,&vEyePt,&vLookatPt,&vUpVec); //compute view matrix
  d3ddevice->SetTransform(D3DTS_VIEW,&matView); //set it
} //SetViewMatrix

void SetProjectionMatrix()
{ //set projection matrix
  D3DXMATRIXA16 matProj;
  FLOAT fAspect=(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
  D3DXMatrixPerspectiveFovRH(&matProj,D3DX_PI/4.0f,fAspect,1.0f,10000.0f);
  d3ddevice->SetTransform(D3DTS_PROJECTION,&matProj);
} //SetProjectionMatrix

void SetAmbientLighting()
{ //set ambient lighting
  //scene ambient light
  d3ddevice->SetRenderState(D3DRS_AMBIENT,0xFFFFFFFF);
  //material ambient color
  D3DMATERIAL9 mtrl;
  ZeroMemory(&mtrl,sizeof(mtrl));
  mtrl.Ambient.r=1.0f;
  mtrl.Ambient.g=1.0f;
  mtrl.Ambient.b=1.0f;
  mtrl.Ambient.a=0.0f;
  d3ddevice->SetMaterial(&mtrl);
} //SetLighting

void SetTextureStates(){ //set texture states to default
  d3ddevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
  d3ddevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
  d3ddevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
  d3ddevice->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
  d3ddevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
  d3ddevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
  d3ddevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
  d3ddevice->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
  d3ddevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
  d3ddevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
} //SetTextureStates

BOOL LoadTextures()
{ //create textures for image storage
  D3DXIMAGE_INFO structImageInfo; //image information
  HRESULT hres=D3DXCreateTextureFromFileEx(d3ddevice,"back.jpg",
    0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_NONE,
    D3DX_DEFAULT,0,&structImageInfo,NULL,&BackgroundTexture);
  hres=D3DXCreateTextureFromFileEx(d3ddevice,"sky.jpg",
    0,0,1,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,D3DX_FILTER_NONE,
    D3DX_DEFAULT,0,&structImageInfo,NULL,&skytexture);
  if(FAILED(hres))
  { //fail and bail
    BackgroundTexture=NULL; return FALSE;
  }
  return TRUE; //succeeded
} //LoadTextures

void LoadSB()
{ //load vertex buffers for skybox
  BILLBOARDVERTEX* negx;
  BILLBOARDVERTEX* posx;
  BILLBOARDVERTEX* negz;
  BILLBOARDVERTEX* posz;
  BILLBOARDVERTEX* top; //vertex buffer data
  float w=SCREEN_WIDTH,h=SCREEN_HEIGHT; //width and height
  if(SUCCEEDED(sbnegx->Lock(0,0,(void**)&negx,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    negx[0].p=D3DXVECTOR3(0,h,0); 
    negx[0].tu=1.0f; negx[0].tv=0.0f;
    negx[1].p=D3DXVECTOR3(0,h,-w);
    negx[1].tu=0.0f; negx[1].tv=0.0f;
    negx[2].p=D3DXVECTOR3(0,0,0);
    negx[2].tu=1.0f; negx[2].tv=1.0f;
    negx[3].p=D3DXVECTOR3(0,0,-w);
    negx[3].tu=0.0f; negx[3].tv=1.0;
    sbnegx->Unlock();
  }  //if

  if(SUCCEEDED(sbposx->Lock(0,0,(void**)&posx,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    posx[0].p=D3DXVECTOR3(w,h,0); 
    posx[0].tu=0.0f; posx[0].tv=0.0f;
    posx[1].p=D3DXVECTOR3(w,h,-w);
    posx[1].tu=1.0f; posx[1].tv=0.0f;
    posx[2].p=D3DXVECTOR3(w,0,0);
    posx[2].tu=0.0f; posx[2].tv=1.0f;
    posx[3].p=D3DXVECTOR3(w,0,-w);
    posx[3].tu=1.0f; posx[3].tv=1.0f;
    sbposx->Unlock();
  }  //if

  if(SUCCEEDED(sbnegz->Lock(0,0,(void**)&negz,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    negz[0].p=D3DXVECTOR3(0,h,-w); 
    negz[0].tu=1.0f; negz[0].tv=0.0f;
    negz[1].p=D3DXVECTOR3(w,h,-w);
    negz[1].tu=0.0f; negz[1].tv=0.0f;
    negz[2].p=D3DXVECTOR3(0,0,-w);
    negz[2].tu=1.0f; negz[2].tv=1.0f;
    negz[3].p=D3DXVECTOR3(w,0,-w);
    negz[3].tu=0.0f; negz[3].tv=1.0f;
    sbnegz->Unlock();
  } //if

  if(SUCCEEDED(sbposz->Lock(0,0,(void**)&posz,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    posz[0].p=D3DXVECTOR3(0,h,0); 
    posz[0].tu=0.0f; posz[0].tv=0.0f;
    posz[1].p=D3DXVECTOR3(w,h,0);
    posz[1].tu=1.0f; posz[1].tv=0.0f;
    posz[2].p=D3DXVECTOR3(0,0,0);
    posz[2].tu=0.0f; posz[2].tv=1.0f;
    posz[3].p=D3DXVECTOR3(w,0,0);
    posz[3].tu=1.0f; posz[3].tv=1.0f;
    sbposz->Unlock();
  }

  if(SUCCEEDED(sbtop->Lock(0,0,(void**)&top,0)))
  { //lock buffer
    //vertex information, first triangle in clockwise order
    top[0].p=D3DXVECTOR3(0,h,-w); 
    top[0].tu=1.0f; top[0].tv=0.0f;
    top[1].p=D3DXVECTOR3(w,h,-w);
    top[1].tu=1.0f; top[1].tv=1.0f;
    top[2].p=D3DXVECTOR3(0,h,0);
    top[2].tu=0.0f; top[2].tv=0.0f;
    top[3].p=D3DXVECTOR3(w,h,0);
    top[3].tu=0.0f; top[3].tv=1.0f;
    sbtop->Unlock();
  }
} //LoadSB

void setupd3d()
{//setup the above matrices
 SetTextureStates(); //set default texture states
 SetWorldMatrix();
 SetViewMatrix();
 SetProjectionMatrix();
 SetAmbientLighting();
 LoadSB(); //load background vertex buffer
}

BOOL PageFlip()
{ //show back buffer
  return SUCCEEDED(d3ddevice->Present(NULL,NULL,NULL,NULL));
} //PageFlip

void DrawBackground()
{ //set vertex buffer to background
  d3ddevice->SetStreamSource(0,sbnegx,0,sizeof(BILLBOARDVERTEX));
  d3ddevice->SetFVF(BILLBOARDVERTEX::FVF); //flexible vertex format
  //draw neg x wall
  d3ddevice->SetTexture(0,BackgroundTexture); //set side texture
  d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

  //draw pos x wall
  d3ddevice->SetStreamSource(0,sbposx,0,sizeof(BILLBOARDVERTEX));
  d3ddevice->SetFVF(BILLBOARDVERTEX::FVF); //flexible vertex format
  d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

  //draw neg z wall
  d3ddevice->SetStreamSource(0,sbnegz,0,sizeof(BILLBOARDVERTEX));
  d3ddevice->SetFVF(BILLBOARDVERTEX::FVF); //flexible vertex format
  d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

  //draw pos z wall
  d3ddevice->SetStreamSource(0,sbposz,0,sizeof(BILLBOARDVERTEX));
  d3ddevice->SetFVF(BILLBOARDVERTEX::FVF); //flexible vertex format
  d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

  //draw neg top wall
  d3ddevice->SetStreamSource(0,sbtop,0,sizeof(BILLBOARDVERTEX));
  d3ddevice->SetFVF(BILLBOARDVERTEX::FVF); //flexible vertex format
  d3ddevice->SetTexture(0,skytexture); //set floor texture
  d3ddevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
} //DrawBackground

BOOL ComposeFrame()
{ //compose a frame of animation 
  d3ddevice->Clear(0L,NULL,D3DCLEAR_TARGET,0,1.0f,0L); //clear render buffer
  if(SUCCEEDED(d3ddevice->BeginScene()))
  { //can start rendering
     DrawBackground();
     d3ddevice->EndScene(); //done rendering
  }
  return TRUE;
} //ComposeFrame

void ProcessFrame()
{ //process a frame of animation
  //InputManager->ProcessKeyboardInput(); //process keyboard input
  //InputManager->ProcessMouseInput(); //process mouse input
  //check for lost graphics device
  if(DeviceLost())
  {   RestoreDevice(); //if device lost, restore it
  }
  ComposeFrame(); //compose a frame in back surface
  PageFlip(); //flip video memory surfaces
} //ProcessFrame