#include "StdAfx.h"
#include "texture.h"
#include <math.h>
#include <stdio.h>

int CTexture::m_maxw = 64;
int CTexture::m_maxh = 64;

void CTexture::OnLostDevice()
{
//  _DestroyTextures();
//  _InitTextures();
}

void CTexture::UpdateDeviceCaps()
{
   int h,w;
   g_D3DObject->GetTextureParms(&h,&w);
   if (w < m_maxw) m_maxw = w;
   if (h < m_maxh) m_maxh = h;
}

CTexture::CTexture(char *filename,D3DCOLOR colorkey)
{
	IDirect3DSurface8 *surf;
	D3DSURFACE_DESC tdesc;

	OutputDebugString("CTexture Create - "); OutputDebugString(filename); OutputDebugString("\n");
	UpdateDeviceCaps();

	//if (FAILED(g_D3DObject->LoadTextureFromFile(filename,&ttext,colorkey,&m_texinfo))) return;
	//ttext->GetLevelDesc(0,&tdesc);
	//SafeRelease(ttext);

	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(filename,&info) != D3D_OK) return;
	tdesc.Format = (D3DFORMAT)21;
	tdesc.Width = info.Width;
	tdesc.Height = info.Height;
	m_texinfo.Width = tdesc.Width;
	m_texinfo.Height = tdesc.Height;
	m_width = tdesc.Width;
	m_height = tdesc.Height;


	if (FAILED(g_D3DObject->MakeScreenSurface(tdesc.Width,tdesc.Height,tdesc.Format,&surf))) return;
	if (FAILED(D3DXLoadSurfaceFromFile(surf,NULL,NULL,filename,
		NULL,D3DX_FILTER_NONE,colorkey,NULL))) return;

	char bff[255];
	sprintf (bff,"CTexture - Format is %i - TFormat is %i Info is %i\n",m_texinfo.Format,tdesc.Format,info.Format);
	OutputDebugString(bff);
	// Get the number of surfaces we will need
	int nrow,ncol;
	ncol = (m_texinfo.Width / m_maxw) + (m_texinfo.Width % m_maxw > 0);
	nrow = (m_texinfo.Height / m_maxh) + (m_texinfo.Height % m_maxh > 0);
	m_nrow = nrow;
	m_ncol = ncol;
	m_numtex = nrow*ncol;
	
	char buff[500];
	sprintf(buff,"CTexture - Image is (%i,%i) size\n",m_texinfo.Height,m_texinfo.Width);
	OutputDebugString(buff);
	sprintf(buff,"CTexture - Creating %i (%i,%i) buffers\n",m_numtex,nrow,ncol);
	OutputDebugString(buff);

	if (m_numtex > 0) {
		m_textures = new IDirect3DTexture8 *[m_numtex];

		for (int x = 0; x < m_numtex; x++) m_textures[x] = NULL;  //INIT
		for (int i = 0; i < nrow; i++) {
		  for (int j = 0; j < ncol; j++){
			int cursurf = (i*ncol)+j;
			char buff[500];
			sprintf(buff,"CTexture - Adding texture number %i (i=%i j=%i)\n",cursurf,i,j);
			OutputDebugString(buff);

			if(FAILED(g_D3DObject->CreateTexture(m_maxw,m_maxh,1,0,tdesc.Format,
												D3DPOOL_MANAGED,&m_textures[cursurf]))) return;
			         
			// generate the image source rectangle
			RECT src;
			//SetRect(&src,j*m_maxw,i*m_maxh,j,bottom)
			src.left=j*m_maxw;
			src.right=(src.left + m_maxw) ;
			src.top=i*m_maxh;
			src.bottom=(src.top + m_maxh);
			if (src.right > (int)m_texinfo.Width) src.right = m_texinfo.Width;
			if (src.bottom > (int)m_texinfo.Height) src.bottom = m_texinfo.Height;

			// get texture surface
			// copy region to texture surface
			//D3DXLoadSurfaceFromSurface(targSurf,NULL,NULL,tempSurf,NULL,&src,D3DX_FILTER_NONE,0);
			//if (FAILED(g_D3DObject->CopyRects(surf,&src,1,tsurf,&pt))) OutputDebugString("CTexture Create - CopyRect Failed\n");
			sprintf(buff,"CTexture - Copying surface to surface - rect = top,left,right,bottom (%i,%i,%i,%i)\n",
				src.top,src.left,src.right,src.bottom);
			OutputDebugString(buff);
			IDirect3DSurface8 *tsurf;
			
			m_textures[cursurf]->GetSurfaceLevel(0,&tsurf);
			D3DXLoadSurfaceFromSurface(tsurf,NULL,NULL,surf,NULL,&src,D3DX_FILTER_NONE,0);
			SafeRelease(tsurf);
		}
	  }
	}
    OutputDebugString("CTexture - Releasing surfaces\n");
	SafeRelease(surf);
//	SafeRelease(tsurf);
    OutputDebugString("CTexture - Duplicating Filename\n");
	m_filename=strdup(filename);
	OutputDebugString("CTexture - Returning\n");
}

CTexture::~CTexture(void)
{
	OutputDebugString("CTexture Destroy - ");
	OutputDebugString(m_filename);
	OutputDebugString("\n");
	for (int x = 0; x < m_numtex; x++) 
		SafeRelease(m_textures[x]);
//	delete []m_textures;
}

void CTexture::Paint(RECT *srcrect, D3DXVECTOR2 *points)
{
//	OutputDebugString("CTexture::Paint - begin\n");
	D3DCOLOR color = 0xFFFFFFFF;
    float rotation = 0;
	D3DXVECTOR2 rotate(0,0), scale(0,0), trans (0,0);
	RECT rect;

	if (srcrect->left >= m_width) {
		OutputDebugString("CTex - Paint - Normalizing\n");
		CopyRect(&rect,srcrect);
		SetRect(srcrect,rect.left-m_width,rect.top,rect.right-m_width,rect.bottom);
	}
	//Find first "X"
    int first = (srcrect->left / m_maxw);
	int last = (srcrect->right / m_maxh) + ((srcrect->right % m_maxh) >0);
	trans.x=points->x;
	SetRect(&rect,srcrect->left % m_maxw,0,m_maxw,m_maxh);
	for (int x=first; x< last ; x++) {		    
		    int p = x % m_ncol;
			trans.y  = 0;
			for (int r=0; r<m_nrow; r++) {
				int spr = p+r*m_ncol;
				trans.y = (int) r*m_maxh;
				trans.y += points->y;
				if ((trans.y + m_maxw) > 800) rect.right = m_maxw-(800-trans.y);
				if (spr < m_numtex)
					g_D3DObject->SpriteDraw(m_textures[spr],&rect,0,0,rotation,&trans,color);
			}
			char buff[500];
			//sprintf (buff,"Adding %i to trans.x\n",rect.right-rect.left);
			//OutputDebugString(buff);
			trans.x += (rect.right-rect.left);
			if (x == m_ncol) {   // Check for edge of texture
				sprintf (buff,"Edge of screen x=%i, right=%i m_ncol=%i m_nrow=%i\n\n",x,m_width%m_maxw,m_ncol,m_nrow);
				OutputDebugString(buff);
				SetRect(&rect, 0, 0, m_width % m_maxw, m_maxh);
			}
			else
				SetRect(&rect,0,0,m_maxw,m_maxh);
		}
//	OutputDebugString("CTexture::Paint - return\n");
}

int CTexture::GetHeight() {return m_height;}
int CTexture::GetWidth() {return m_width;}