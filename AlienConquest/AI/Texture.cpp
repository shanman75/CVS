#include "StdAfx.h"
#include "texture.h"
#include <math.h>
#include <stdio.h>

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

CTexture::CTexture(char *filename,D3DCOLOR colorkey,RECT *srect, int maxw, int maxh)
{
	IDirect3DSurface8 *surf;
	D3DSURFACE_DESC tdesc;
	//char buff[250];

	m_maxw = maxw;
	m_maxh = maxh;

	m_filename = strdup(filename);

	//sprintf (buff,"CTexture::Create - loading %s RECT=%i\n",filename,srect);
	//OutputDebugString(buff);
	UpdateDeviceCaps();

	D3DXIMAGE_INFO info;
	if (D3DXGetImageInfoFromFile(filename,&info) != D3D_OK) return;
	tdesc.Format = (D3DFORMAT)21;
	
	m_width = srect == NULL ? info.Width : srect->right - srect->left;
	m_height = srect == NULL ? info.Height : srect->bottom - srect->top;
	

	if (FAILED(g_D3DObject->MakeScreenSurface(m_width,m_height,tdesc.Format,&surf))) return;
	if (FAILED(D3DXLoadSurfaceFromFile(surf,NULL,NULL,filename,
		srect,D3DX_FILTER_NONE,colorkey,NULL))) return;

	//char bff[255];
	//sprintf (bff,"CTexture - Format is %i - TFormat is %i Info is %i\n",m_texinfo.Format,tdesc.Format,info.Format);
	//OutputDebugString(bff);
	// Get the number of surfaces we will need
	int nrow,ncol;
	ncol = (m_width / m_maxw) + (m_width % m_maxw > 0);
	nrow = (m_height / m_maxh) + (m_height % m_maxh > 0);
	m_nrow = nrow;
	m_ncol = ncol;
	m_numtex = nrow*ncol;
	
	//sprintf(buff,"CTexture - Image is (%i,%i) size\n",m_height,m_width);
	//OutputDebugString(buff);
	//sprintf(buff,"CTexture - Creating %i (%i,%i) buffers\n",m_numtex,nrow,ncol);
	//OutputDebugString(buff);

	if (m_numtex > 0) {
		m_textures = new IDirect3DTexture8 *[m_numtex];

		for (int x = 0; x < m_numtex; x++) m_textures[x] = NULL;  //INIT
		for (int i = 0; i < nrow; i++) {
		  for (int j = 0; j < ncol; j++){
			int cursurf = (i*ncol)+j;
//			char buff[500];
			
			//sprintf(buff,"CTexture - Adding texture number %i (i=%i j=%i)\n",cursurf,i,j);
			//OutputDebugString(buff);

			if(FAILED(g_D3DObject->CreateTexture(m_maxw,m_maxh,1,0,tdesc.Format,
												D3DPOOL_MANAGED,&m_textures[cursurf]))) return;
			         
			// generate the image source rectangle
			RECT src;
			//SetRect(&src,j*m_maxw,i*m_maxh,j,bottom)
			src.left=j*m_maxw;
			src.right=(src.left + m_maxw) ;
			src.top=i*m_maxh;
			src.bottom=(src.top + m_maxh);
			if (src.right > (int)m_width) src.right = m_width;
			if (src.bottom > (int)m_height) src.bottom = m_height;

			//sprintf(buff,"CTexture - Copying surface to surface - rect = top,left,right,bottom (%i,%i,%i,%i)\n",
			//	src.top,src.left,src.right,src.bottom);
			//OutputDebugString(buff);
			
			IDirect3DSurface8 *tsurf;
			
			m_textures[cursurf]->GetSurfaceLevel(0,&tsurf);
			D3DXLoadSurfaceFromSurface(tsurf,NULL,NULL,surf,NULL,&src,D3DX_FILTER_NONE,0);
			SafeRelease(tsurf);
		}
	  }
	}
	SafeRelease(surf);
}

CTexture::~CTexture(void)
{
	//OutputDebugString("CTexture Destroy - ");
	//OutputDebugString(m_filename);
	//OutputDebugString("\n");
	for (int x = 0; x < m_numtex; x++) 
		SafeRelease(m_textures[x]);
	delete []m_textures;
}

void CTexture::Paint(D3DXVECTOR2 *points)
{
	D3DCOLOR color = 0xFFFFFFFF;
    float rotation = 0;
	D3DXVECTOR2 rotate(0,0), scale(0,0), trans (0,0);
	RECT rect;

	trans.x=points->x;
	for (int mx = 0; mx < m_ncol; mx++) {
		trans.y=points->y;
		for (int my = 0; my < m_nrow; my++) {
		  SetRect(&rect,0,0,m_maxh,m_maxw);
		  int spr = (my*m_ncol)+mx;
		  if (spr < m_numtex) {
			g_D3DObject->SpriteDraw(m_textures[spr],&rect,0,0,0,&trans,color);
		  }
		  else {OutputDebugString("CTexture - Paint....sprite out of range error\n");}
		  trans.y+=m_maxh;
		}
		trans.x+=m_maxw;
	}
}

void CTexture::Paint(RECT *srcrect, D3DXVECTOR2 *points)
{
	D3DCOLOR color = 0xFFFFFFFF;
    float rotation = 0;
	D3DXVECTOR2 rotate(0,0), scale(0,0), trans (0,0);
	RECT rect;
//	char buff[500];
//		OutputDebugString("CTex - Paint - Begin\n");
	if (srcrect->left >= m_width) {
//		sprintf(buff,"Normalizing, srcrect->left=%i\n",srcrect->left);
//		OutputDebugString(buff);
		CopyRect(&rect,srcrect);
		int mw = rect.right-rect.left;
		int mt = rect.left%m_width;
		SetRect(srcrect,mt,rect.top,mt+mw,rect.bottom);
	}
//	sprintf(buff,"Painting srcrect->left now =%i\n",srcrect->left);
//	OutputDebugString(buff);
//	g_D3DObject->DrawTextStr(150,150,D3DCOLOR_XRGB(255,0,255),buff);

	//Find first "X"
	int curx = srcrect->left;
	trans.x = points->x;
	int mx = (int) (curx / m_maxw)%m_ncol;   // The "X" Coord of the Square
	int ax = (int) (curx % m_maxw);	  // The "X" Coord of the point in the square (first time)
	while (curx <= srcrect->right) {
		int ar = (int) m_maxw;
		//if ((srcrect->right - curx) < ar) ar = srcrect->right-curx;  // sub-rect at the end
		if ((mx == (m_ncol-1)) && ((m_width%m_maxw)<ar)) ar = m_width%m_maxw; //blank space at the end
		
		int cury = srcrect->top;
		trans.y = points->y;
		int my = (int) (cury / m_maxh)%m_nrow;
        int ay = (int) (cury % m_maxh);
		while (cury <= srcrect->bottom) {
		  int ab = (int) m_maxh;
		  //if ((srcrect->bottom-cury)<ab) ab = srcrect->right - cury;
		  if ((my == (m_nrow-1)) && ((m_height%m_maxh)<ab)) ab = m_height %m_maxh;
		  SetRect(&rect,ax,ay,ar,ab);
		  int spr = (mx)+(my*m_ncol);
		  if (spr < m_numtex) {
			g_D3DObject->SpriteDraw(m_textures[spr],&rect,0,0,0,&trans,color);
		  }
		  else {OutputDebugString("CTexture - Paint....sprite out of range error\n");}
//		  char buff[500];
		  cury += abs(rect.bottom - rect.top) + (rect.bottom == rect.top);
		  trans.y += abs(rect.bottom - rect.top);
		  my = (my+1)%m_nrow;
		  ay=0;
		}
//		  sprintf(buff,"Curx = %i, Cury = %i rect-(%i,%i,%i,%i) trans(%i,%i) spr=%i\n",curx,cury,
//			  rect.left,rect.top,rect.right,rect.bottom,trans.x,trans.y,0);
//		  OutputDebugString(buff);
		mx = (mx+1)%m_ncol;
		trans.x += abs(rect.right - rect.left) ;
	    curx    += abs(rect.right - rect.left)+ (rect.right == rect.left);
		ax	    = 0;
	}
}

int CTexture::GetHeight() {return m_height;}
int CTexture::GetWidth() {return m_width;}