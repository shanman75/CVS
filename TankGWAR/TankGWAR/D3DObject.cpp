#include "StdAfx.h"
#include "d3dobject.h"
#include <stdio.h>
#include "MainWnd.h"

//#include "objmgr.h"
//#include "text.h"

D3DObject *g_D3DObject;							// Main D3D Object
BOOL D3DObject::m_initfonts;
//CText m_text;

D3DObject::D3DObject(void) : m_d3d9(NULL), 
                             m_d3ddevice9(NULL),
							 m_d3dbackbuffer9(NULL),
							 m_pBackgroundSurface(NULL),
							 m_cnt(0)
{
   _InitD3D9();
}

D3DObject::~D3DObject(void)
{
  OutputDebugString("D3DObject - Delete\n");
  SafeRelease(m_pd3dxSprite);
  m_d3ddevice9->ShowCursor(TRUE);
  ShowCursor(TRUE);
  SafeRelease(m_pBackgroundSurface);
  SafeRelease(m_d3dbackbuffer9);
  SafeRelease(m_d3ddevice9);
  SafeRelease(m_d3d9);
  SafeRelease(pFont);
}

int D3DObject::_InitD3D9(void)
{
//  HRESULT retval;

  pFont = NULL;

  if ((m_d3d9=Direct3DCreate9(D3D_SDK_VERSION)) ==NULL) return FALSE;

//  if (m_d3d8->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
  ZeroMemory (&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS)); //null it out

  curmode.Height = 0;
  curmode.Width = 0;
  int m = 0;

  while (!(curmode.Width == WIDTH || curmode.Height == HEIGHT))
  {
    D3DFORMAT dfmt;
	dfmt = D3DFMT_X8R8G8B8;
	m_d3d9->EnumAdapterModes(D3DADAPTER_DEFAULT,dfmt,m++,&curmode);
  }

  m_d3dpp.BackBufferWidth=curmode.Width; //width
  m_d3dpp.BackBufferHeight=curmode.Height; //height
  m_d3dpp.BackBufferFormat=curmode.Format; //color mode
  m_d3dpp.BackBufferCount=1; //one back buffer
  m_d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
  m_d3dpp.SwapEffect=D3DSWAPEFFECT_FLIP; //flip pages
  //m_d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD; // Try it..
//  m_d3dpp.SwapEffect             = D3DSWAPEFFECT_COPY_VSYNC;
  m_d3dpp.hDeviceWindow=NULL; //full screen
  m_d3dpp.Windowed=false; //full screen

  if (1) {  
  m_d3dpp.Windowed = true;
  m_d3dpp.hDeviceWindow = g_hWnd;
  m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
  SetWindowPos(
			g_hWnd,
			NULL,
			0,
			0,
			700,
			500,
			SWP_NOZORDER | SWP_SHOWWINDOW
		);
}

  m_d3dpp.EnableAutoDepthStencil=true; //not needed
  m_d3dpp.AutoDepthStencilFormat=D3DFMT_D16; //not needed
  m_d3dpp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //can lock buffer
  m_d3dpp.FullScreen_RefreshRateInHz =D3DPRESENT_RATE_DEFAULT;
  //m_d3dpp.FullScreen_RefreshRateInHz =70;

//  m_d3dpp.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE ;
//  m_d3dpp.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT ;
  m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  //m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


/*
if (m_d3d8->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
	  D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_d3ddevice8) != D3D_OK)
	  return FALSE;
*/
  if (m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,g_hWnd,
	  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,&m_d3dpp,&m_d3ddevice9) != D3D_OK)
	  return FALSE;

  m_d3ddevice9->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE);
  m_d3ddevice9->SetRenderState ( D3DRS_LIGHTING, FALSE);
  m_d3ddevice9->SetRenderState ( D3DRS_ZENABLE, FALSE);

  m_d3ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  m_d3ddevice9->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  m_d3ddevice9->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);

//  m_d3ddevice9->SetVertexShader( D3DFVF_CVERTEX );


  if (m_d3ddevice9->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_d3dbackbuffer9) != D3D_OK) {
    m_d3dbackbuffer9=NULL; return FALSE;
  }

  ShowCursor(FALSE);
  m_d3ddevice9->ShowCursor(FALSE);
  m_d3ddevice9->GetDeviceCaps(&m_d3dcps);

  D3DXCreateSprite(m_d3ddevice9,&m_pd3dxSprite);

  char outstr[5000];
  sprintf (outstr,"Device Statistics\nMaxTexHeight: %i, MaxTexWidth: %i\n",m_d3dcps.MaxTextureHeight,m_d3dcps.MaxTextureWidth);
  OutputDebugString(outstr);
  return TRUE;
}

// Paint a frame
int D3DObject::PaintFrame(IDirect3DSurface9* in_Frame)
{
	DeviceLost();

	m_d3ddevice9->BeginScene();
	m_d3ddevice9->UpdateSurface(in_Frame,NULL,m_d3dbackbuffer9,NULL);
	//m_d3ddevice9->CopyRects(in_Frame,NULL,0,m_d3dbackbuffer8,NULL);
	m_d3ddevice9->EndScene();
	m_d3ddevice9->Present(NULL,NULL,NULL,NULL);
	return 0;
}

int D3DObject::MakeScreenSurface(int width, int height, D3DFORMAT format, IDirect3DSurface9** dest_surf)
{
	if (width == 0) width = m_d3dpp.BackBufferWidth;
	if (height == 0) height = m_d3dpp.BackBufferHeight;
	if (format == D3DFMT_UNKNOWN) format = m_d3dpp.BackBufferFormat;

	return m_d3ddevice9->CreateOffscreenPlainSurface(width,
		height,
		format,
		D3DPOOL_DEFAULT,
		dest_surf,
		NULL);
	//return m_d3ddevice9->CreateImageSurface(width,height,
	//	format,dest_surf);
}

BOOL D3DObject::DeviceLost(){ //check for lost device
  if (m_d3ddevice9->TestCooperativeLevel()!=D3D_OK)
  {
	OutputDebugString("D3DObject::DeviceLost Restoring Surfaces\n");
	m_d3dbackbuffer9->Release();
	m_d3dbackbuffer9=NULL;
	if(m_d3ddevice9->Reset(&m_d3dpp)!=D3D_OK)
		return FALSE;
	//get new surfaces
	if(m_d3ddevice9->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_d3dbackbuffer9)!=D3D_OK)
		return FALSE;
	m_pd3dxSprite->OnLostDevice();
  }
  return D3D_OK;
} //DeviceLost

int D3DObject::LoadSurfaceFromFile (char *fname, IDirect3DSurface9 **surf, D3DCOLOR ckey,D3DXIMAGE_INFO *SrcInfo, D3DFORMAT fmt)
{
	D3DXIMAGE_INFO info;

	if (!SrcInfo) SrcInfo = &info;

	int retval;
	OutputDebugString("D3DObject::LoadSurfaceFromFile - ");
    OutputDebugString(fname);
	OutputDebugString("\n");
	if (!fmt) fmt = D3DFMT_UNKNOWN;
	if (D3DXGetImageInfoFromFile(fname,SrcInfo) != D3D_OK) return D3DERR_INVALIDCALL;
//	if (MakeScreenSurface(SrcInfo->Width,SrcInfo->Height,D3DFMT_UNKNOWN,surf) != D3D_OK) return FALSE;
	if (MakeScreenSurface(SrcInfo->Width,SrcInfo->Height,fmt,surf) != D3D_OK) return FALSE;

//	retval = D3DXLoadSurfaceFromFile(*surf,NULL,NULL, //copy to surface
//      fname,NULL,D3DFMT_UNKNOWN,ckey,NULL); //using default settings
	retval = D3DXLoadSurfaceFromFile(*surf,NULL,NULL, //copy to surface
      fname,NULL,fmt,ckey,NULL); //using default settings
	OutputDebugString("D3DObject::LoadSurfaceFromFile - OK\n");
	return D3D_OK;
}

int D3DObject::CopyRects(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,
				  IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPointsArray)
{
	//char buff[500];

	//sprintf(buff,"D3DObject::CopyRects- (%i,%i,%i,%i)\n",pSourceRectsArray[0].left,pSourceRectsArray[0].right,
	//													pSourceRectsArray[0].top,pSourceRectsArray[0].bottom);
	//OutputDebugString(buff);
//	return m_d3ddevice9->CopyRects(pSourceSurface,pSourceRectsArray,cRects,
//				  pDestinationSurface, pDestPointsArray);
	return m_d3ddevice9->UpdateSurface(pSourceSurface,pSourceRectsArray,pDestinationSurface,pDestPointsArray);
}

int D3DObject::CopyBackTexture(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,
				  CONST POINT* pDestPointsArray, int DEST)
{
	if (m_pVidmemTex1 != NULL) {
	 IDirect3DSurface9* pDestinationSurface;
	 if (m_pVidmemTex1->GetSurfaceLevel(2-DEST,&pDestinationSurface) != D3D_OK) return FALSE;
	 m_d3ddevice9->UpdateSurface(pSourceSurface,pSourceRectsArray,
				  pDestinationSurface, pDestPointsArray);
	 pDestinationSurface->Release();
	 return D3D_OK;
	}
	return D3D_OK;
}

int D3DObject::LoadTextureFromFile(char *fname, IDirect3DTexture9 **texture, D3DCOLOR colorkey,D3DXIMAGE_INFO *SrcInfo)
{
	D3DXIMAGE_INFO info;
	HRESULT hr;

	if (D3DXGetImageInfoFromFile(fname,&info) != D3D_OK) return D3DERR_INVALIDCALL;
	hr = D3DXCreateTextureFromFileEx(m_d3ddevice9,fname,curmode.Width*2,curmode.Height,
			1, 0L, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
			D3DX_FILTER_NONE , D3DX_DEFAULT  , 
			colorkey, SrcInfo , NULL, texture);

	if(FAILED(hr)) 
	{
		TCHAR buff[400]= "NONE";
//		D3DXGetErrorString(hr, buff, 400);
		OutputDebugString("--[ unable to load texture because of: ");
		OutputDebugString(buff);
		wsprintf(buff, "texture file name: %s ]--", fname);
		OutputDebugString(buff);
		*texture = NULL;
	}
	else
	{
		OutputDebugString("-- [ loaded texture from file: ");
		OutputDebugString(fname);
		OutputDebugString("\n");
	}
	return D3D_OK;
}

int D3DObject::PaintText ()
{
//  OutputDebugString("D3DObject::Test - begin\n"); 

	static float newfps = 30;
	static float fps = 30;

	/*
	if (m_timer.PeekTime() > 1000) 
	{
		m_timer.Reset();
		fps=newfps;
			///m_timer.GetTime()*1000;
		newfps=0;
	}
	newfps++;
    */

	static D3DXVECTOR2 pnt (150,300);

	static char outstr2[500];
//	sprintf(outstr2,"FPS = %.2f NUM Objects=%i\n",fps,g_ObjMgr->GetNumObj());
//	m_text.DrawString(outstr2,pnt);
	//if (text) text->DrawScore(5000,pnt);
	DrawTextStr(150,300,D3DCOLOR_XRGB(255,0,255),outstr2);
//	delete outstr2;

  //}
return D3D_OK;
}

int D3DObject::_InitFonts() {

	HRESULT hr;
	LOGFONT fn;

	//memset(&fn,0,sizeof(LOGFONT));
	//strcpy (fn.lfFaceName,"DROSS2.TTF");
	//fn.lfHeight = 48;
	//fn.lfWeight = 200;
	//fn.lfCharSet = DEFAULT_CHARSET;

	//// Get a handle for the font to use
	////HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);
	//HFONT hFont = (HFONT) CreateFontIndirect(&fn);

	pFont = NULL;

	// Create the D3DX Font
	D3DXFONT_DESC fdesc;
	memset (&fdesc,0,sizeof(fdesc));
	strcpy (fdesc.FaceName,"Arial");
	fdesc.Weight = 1000;
	fdesc.Height = 42;
	hr = D3DXCreateFontIndirect(m_d3ddevice9, &fdesc, &pFont);
	if(FAILED(hr))
		return FALSE;
	m_initfonts=TRUE;
	return TRUE;
}

int D3DObject::DrawTextStr(int x, int y, DWORD color, const TCHAR * str)
{

	if (!m_initfonts) _InitFonts();
	// Rectangle where the text will be located
	RECT TextRect = { x, y, 0, 0 };

	// Inform font it is about to be used
	//pFont->Begin();

	// Calculate the rectangle the text will occupy
	pFont->DrawText( NULL, str, -1, &TextRect, DT_CALCRECT, 0);

	// Outputn the text, left aligned
	pFont->DrawText( NULL, str, -1, &TextRect, DT_LEFT, color );

	// Finish up drawing
	//pFont->End();
	return TRUE;
}

void D3DObject::BlitRect(LPDIRECT3DTEXTURE9 lpSrc,
              float left, float top,
              float right, float bottom,
              D3DCOLOR col,float z)
{
    // calculate rhw
	LPDIRECT3DDEVICE9 lpDevice = m_d3ddevice9;

    float rhw=1.0f/(z*990.0f+10.0f);

    // set up rectangle

    D3DTLVERTEX verts[4];
    verts[0]=D3DTLVERTEX(D3DXVECTOR3(left-0.5f,  top-0.5f,    z),rhw,col,0.0f,0.0f); 
    verts[1]=D3DTLVERTEX(D3DXVECTOR3(right-0.5f, top-0.5f,    z),rhw,col,1.0f,0.0f);
    verts[2]=D3DTLVERTEX(D3DXVECTOR3(right-0.5f, bottom-0.5f, z),rhw,col,1.0f,1.0f); 
    verts[3]=D3DTLVERTEX(D3DXVECTOR3(left-0.5f,  bottom-0.5f, z),rhw,col,0.0f,1.0f);
    // set the texture
    lpDevice->SetTexture(0,lpSrc);
    // configure shader for vertex type
    //lpDevice->SetVertexShader(D3DFVF_TLVERTEX);
    // draw the rectangle
    lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,4,verts,sizeof(D3DTLVERTEX));
}

void D3DObject::CopySurfaceToTexture(IDirect3DSurface9 *sf, IDirect3DTexture9 *tx)
{
   IDirect3DSurface9 *dst;
   tx->GetSurfaceLevel(0,&dst);
   m_d3ddevice9->UpdateSurface(sf,0,dst,0);
   if (dst) 
   dst->Release();

}

HRESULT D3DObject::RenderText()
{
	if (m_pD3DXFont==NULL) return FALSE;
    D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,0);
    D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
    TCHAR szMsg[MAX_PATH] = TEXT("");
    RECT rct;
    ZeroMemory( &rct, sizeof(rct) );       

//    m_pD3DXFont->Begin();
    rct.left   = 2;
    rct.right  = curmode.Width - 20;

    // Output display stats
    INT nNextLine = 50; 
/*
	char * m_strFrameStats = "50";
    lstrcpy( szMsg, m_strFrameStats );
    rct.top = nNextLine; rct.bottom = rct.top + 20;    
    m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );
	nNextLine += 14;

    char * m_strDeviceStats = "blah";
	lstrcpy( szMsg, m_strDeviceStats );
    rct.top = nNextLine; rct.bottom = rct.top + 20;    
    m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );
	nNextLine += 14;
*/
    // Output help
    nNextLine = curmode.Height; 

    lstrcpy( szMsg, TEXT("Press 'F1' to toggle text") );
    nNextLine -= 14; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    m_pD3DXFont->DrawText( NULL, szMsg, -1, &rct, 0, fontColor );

    lstrcpy( szMsg, TEXT("Press 'Ctrl-O' to load image") );
    nNextLine -= 14; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    m_pD3DXFont->DrawText( NULL, szMsg, -1, &rct, 0, fontColor );

//    m_pD3DXFont->End();

    return S_OK;
}

void D3DObject::GetTextureParms(int *max_height, int *max_width )
{
	*max_height = m_d3dcps.MaxTextureHeight;
	*max_width = m_d3dcps.MaxTextureWidth;
}

int D3DObject::CreateTexture(UINT Width,UINT Height,UINT  Levels,DWORD Usage,D3DFORMAT Format,
							 D3DPOOL Pool,IDirect3DTexture9** ppTexture)
{
	if (Format == 0) Format = curmode.Format;
	return m_d3ddevice9->CreateTexture(Width,Height,Levels,
		                               Usage,Format,Pool,ppTexture,NULL);
}

int D3DObject::SpriteDraw( IDirect3DTexture9 *texture, RECT *rect, D3DXVECTOR2 *scale, D3DXVECTOR2 *rotate, 
						  float rotation,
						  D3DXVECTOR2 *trans, D3DCOLOR color)
{	
   D3DXMATRIX transform;
   D3DXMatrixTransformation2D(&transform,
	   scale,          // Scaling Center D3DXVECTOR2 
	   NULL,            // Scaling Rotation factor
	   NULL,           // Scaling Matrix  D3DXVECTOR2 
	   rotate,           // Rotation Center D3DXVECTOR2
	   rotation,			   // Rotation (in radians)
	   trans           // Translation D3DXVECTOR2
	   );


   m_pd3dxSprite->SetTransform(&transform);
   return m_pd3dxSprite->Draw(texture,rect,NULL,NULL,
	                color);
   //return m_pd3dxSprite->Draw( texture, rect, 
	//                           scale, rotate, rotation, trans, 
	//						   color );
}

void D3DObject::BeginPaint()
{
	DeviceLost();
	m_d3ddevice9->BeginScene();
    m_d3ddevice9->Clear( 0, NULL, D3DCLEAR_TARGET, 
          D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
    m_pd3dxSprite->Begin(D3DXSPRITE_DONOTSAVESTATE);
}

void D3DObject::EndPaint()
{	m_pd3dxSprite->End();
	m_d3ddevice9->EndScene();  
	m_d3ddevice9->Present(NULL,NULL,NULL,NULL);
}