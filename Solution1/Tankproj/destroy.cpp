/****************************************************************
destroextern LPDIRECT3DVERTEXBUFFER9y.cpp

This is a file for the exiting of direct 3d and releasing
textures.

last modified: April 8, 2004
****************************************************************/

#include "destroy.h"

extern IDirect3D9 *d3dcomobj;
extern IDirect3DDevice9 *d3ddevice;
extern D3DPRESENT_PARAMETERS presparams;
//extern IDirect3DSurface9* d3dbackbuffer;
//extern IDirect3DSurface9* testscreen;
extern LPDIRECT3DVERTEXBUFFER9 sbnegx;
extern LPDIRECT3DVERTEXBUFFER9 sbposx;
extern LPDIRECT3DVERTEXBUFFER9 sbnegz;
extern LPDIRECT3DVERTEXBUFFER9 sbposz;
extern LPDIRECT3DVERTEXBUFFER9 sbtop;

void clean()
{//perform cleaning tasks
 ReleaseD3D(); //release Direct3D
 PostQuitMessage(0); //this is the last thing to do on exit
}

void ReleaseD3D()
{ //release d3d
//  if(d3dbackbuffer)d3dbackbuffer->Release(); //release back buffer
  if(sbnegx)sbnegx->Release(); //release vertex buffers
  if(sbposx)sbposx->Release();
  if(sbnegz)sbnegz->Release();
  if(sbposz)sbposz->Release();
  if(sbtop)sbtop->Release();
  if(d3ddevice)d3ddevice->Release(); //release device
  if(d3dcomobj)d3dcomobj->Release(); //release d3d object
} //ReleaseD3D
