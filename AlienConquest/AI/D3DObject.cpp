#include "StdAfx.h"
#include "d3dobject.h"
#include "stdio.h"

D3DObject *g_D3DObject;							// Main D3D Object

D3DObject::D3DObject(void)
: m_d3d8(NULL)
, m_d3ddevice8(NULL)
, m_d3dbackbuffer8(NULL)
, m_pBackgroundSurface(NULL)
, m_cnt(0)
{
   _InitD3D8();
}

D3DObject::~D3DObject(void)
{
  m_d3ddevice8->ShowCursor(TRUE);
  ShowCursor(TRUE);
  if(m_pBackgroundSurface != NULL )m_pBackgroundSurface->Release(); //release background
  if(m_d3dbackbuffer8 != NULL)     m_d3dbackbuffer8->Release(); //release back buffer
  if(m_d3ddevice8 != NULL)         m_d3ddevice8->Release(); //release device
  if(m_d3d8 != NULL)			   m_d3d8->Release(); //release d3d object
}

int D3DObject::_InitD3D8(void)
{
//  HRESULT retval;

  if ((m_d3d8=Direct3DCreate8(D3D_SDK_VERSION)) ==NULL) return FALSE;

//  if (m_d3d8->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
  ZeroMemory (&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS)); //null it out

  curmode.Height = 0;
  curmode.Width = 0;
  int m = 0;

  while (!(curmode.Width == WIDTH || curmode.Height == HEIGHT))
  {
	m_d3d8->EnumAdapterModes(D3DADAPTER_DEFAULT,m++,&curmode);
  }

  m_d3dpp.BackBufferWidth=curmode.Width; //width
  m_d3dpp.BackBufferHeight=curmode.Height; //height
  m_d3dpp.BackBufferFormat=curmode.Format; //color mode
  m_d3dpp.BackBufferCount=1; //one back buffer
  m_d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
//  m_d3dpp.SwapEffect=D3DSWAPEFFECT_FLIP; //flip pages
  m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  m_d3dpp.hDeviceWindow=NULL; //full screen
  m_d3dpp.Windowed=FALSE; //full screen
  m_d3dpp.EnableAutoDepthStencil=FALSE; //not needed
  m_d3dpp.AutoDepthStencilFormat=D3DFMT_D16; //not needed
  m_d3dpp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //can lock buffer
  m_d3dpp.FullScreen_RefreshRateInHz =D3DPRESENT_RATE_DEFAULT;
  m_d3dpp.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE ;
  m_d3dpp.FullScreen_PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT ;

/*
if (m_d3d8->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
	  D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_d3ddevice8) != D3D_OK)
	  return FALSE;
*/
  if (m_d3d8->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
	  D3DCREATE_SOFTWARE_VERTEXPROCESSING ,&m_d3dpp,&m_d3ddevice8) != D3D_OK)
	  return FALSE;

  D3DXCreateSprite(m_d3ddevice8,&m_pd3dxSprite);

  m_d3ddevice8->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE);
  m_d3ddevice8->SetRenderState ( D3DRS_LIGHTING, FALSE);
  m_d3ddevice8->SetRenderState ( D3DRS_ZENABLE, FALSE);

  m_d3ddevice8->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  m_d3ddevice8->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  m_d3ddevice8->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

  m_d3ddevice8->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  m_d3ddevice8->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  m_d3ddevice8->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  m_d3ddevice8->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);

  m_d3ddevice8->SetVertexShader( D3DFVF_CVERTEX );


  if (m_d3ddevice8->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&m_d3dbackbuffer8) != D3D_OK) {
    m_d3dbackbuffer8=NULL; return FALSE;
  }

  ShowCursor(FALSE);
  m_d3ddevice8->ShowCursor(FALSE);

  D3DCAPS8 cps;
  m_d3ddevice8->GetDeviceCaps(&cps);
  char outstr[5000];
  sprintf (outstr,"Device Statistics\nMaxTexHeight: %i, MaxTexWidth: %i\n",cps.MaxTextureHeight,cps.MaxTextureWidth);
  OutputDebugString(outstr);
  return TRUE;
}

// Paint a frame
int D3DObject::PaintFrame(IDirect3DSurface8* in_Frame)
{
	DeviceLost();

	m_d3ddevice8->BeginScene();
	m_d3ddevice8->CopyRects(in_Frame,NULL,0,m_d3dbackbuffer8,NULL);
	m_d3ddevice8->EndScene();
	m_d3ddevice8->Present(NULL,NULL,NULL,NULL);
	return 0;
}

int D3DObject::MakeScreenSurface(int width, int height, D3DFORMAT format, IDirect3DSurface8** dest_surf)
{
	if (width == 0) width = m_d3dpp.BackBufferWidth;
	if (height == 0) height = m_d3dpp.BackBufferHeight;
	if (format == D3DFMT_UNKNOWN) format = m_d3dpp.BackBufferFormat;
	return m_d3ddevice8->CreateImageSurface(width,height,
		format,dest_surf);
}

BOOL D3DObject::DeviceLost(){ //check for lost device
  if (m_d3ddevice8->TestCooperativeLevel()!=D3D_OK)
  {
	m_d3dbackbuffer8->Release();
	m_d3dbackbuffer8=NULL;
	if(m_d3ddevice8->Reset(&m_d3dpp)!=D3D_OK)
		return FALSE;
	//get new surfaces
	if(m_d3ddevice8->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&m_d3dbackbuffer8)!=D3D_OK)
		return FALSE;
  }
  return D3D_OK;
} //DeviceLost

int D3DObject::LoadSurfaceFromFile (char *fname, IDirect3DSurface8 **surf, D3DCOLOR ckey)
{
	D3DXIMAGE_INFO info;
	int retval;
    OutputDebugString(fname);
	OutputDebugString("\n");
	if (D3DXGetImageInfoFromFile(fname,&info) != D3D_OK) return D3DERR_INVALIDCALL;
	if (MakeScreenSurface(info.Width,info.Height,D3DFMT_UNKNOWN,surf) != D3D_OK) return FALSE;

	retval = D3DXLoadSurfaceFromFile(*surf,NULL,NULL, //copy to surface
      fname,NULL,D3DFMT_UNKNOWN,ckey,NULL); //using default settings
	return D3D_OK;
}

int D3DObject::CopyRects(IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,
				  IDirect3DSurface8* pDestinationSurface, CONST POINT* pDestPointsArray)
{
	return m_d3ddevice8->CopyRects(pSourceSurface,pSourceRectsArray,cRects,
				  pDestinationSurface, pDestPointsArray);
}

int D3DObject::CopyBackTexture(IDirect3DSurface8* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,
				  CONST POINT* pDestPointsArray, int DEST)
{
	if (m_pVidmemTex1 != NULL) {
	 IDirect3DSurface8* pDestinationSurface;
	 if (m_pVidmemTex1->GetSurfaceLevel(2-DEST,&pDestinationSurface) != D3D_OK) return FALSE;
	 m_d3ddevice8->CopyRects(pSourceSurface,pSourceRectsArray,cRects,
				  pDestinationSurface, pDestPointsArray);
	 pDestinationSurface->Release();
	 return D3D_OK;
	}
	return D3D_OK;
}

int D3DObject::LoadTextureFromFile(char *fname, IDirect3DTexture8 **texture, D3DCOLOR colorkey,D3DXIMAGE_INFO *SrcInfo)
{
	D3DXIMAGE_INFO info;
	HRESULT hr;

	if (D3DXGetImageInfoFromFile(fname,&info) != D3D_OK) return D3DERR_INVALIDCALL;
	hr = D3DXCreateTextureFromFileEx(m_d3ddevice8,fname,curmode.Width*2,curmode.Height,
			1, 0L, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
			D3DX_FILTER_NONE , D3DX_DEFAULT  , 
			colorkey, SrcInfo , NULL, texture);

	if(FAILED(hr)) 
	{
		TCHAR buff[400];
		D3DXGetErrorString(hr, buff, 400);
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

int D3DObject::Test (IDirect3DTexture8 **m_lpTexture, D3DXIMAGE_INFO text_desc[])
{

  static CTimer mytime;
  float SCALE = 0.023;

//  for (int x = 1; x < 70; x++) {
  static float mx = 0;
  mx += (mytime.UpdateGetTime() * SCALE);
  int m_x = (int)mx % 1500;

  m_d3ddevice8->BeginScene();
  m_d3ddevice8->Clear( 0, NULL, D3DCLEAR_TARGET, 
          D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );

	D3DXVECTOR2 trans(0,0);
	RECT SrcRect, *pSrcRect = &SrcRect;

	m_pd3dxSprite->Begin();
	trans.x=0;

	float sky = 0.12;
	float groud = 0.65;
	float below = 1.68;
	SetRect(&SrcRect,0+m_x*sky,0,curmode.Width+m_x*sky,600);
	trans.y = 0;
	m_pd3dxSprite->Draw( m_lpTexture[0], &SrcRect, 0, 0, 0, &trans, 0xFFFFFFFF );
	SetRect(&SrcRect,0+m_x*groud,0,curmode.Width+m_x*groud,600);
	trans.y=0;
	m_pd3dxSprite->Draw( m_lpTexture[1], &SrcRect, 0, 0, 0, &trans, 0xFFFFFFFF );
	SetRect(&SrcRect,0+m_x*below,0,curmode.Width+m_x*below,50);
	trans.y=curmode.Height-50;
	m_pd3dxSprite->Draw( m_lpTexture[2], &SrcRect, 0, 0, 0, &trans, 0xFFFFFFFF );
	m_pd3dxSprite->End();

	static float newfps = 50;
	static float fps = 50;
	if (m_timer.UpdatePeekTime() > 1000) 
	{
		fps=newfps;
		newfps=0;
		m_timer.Reset();
	}
	newfps++;

	/*
	D3DCAPS8 caps;
	char *outstr = new char[500];
	m_d3ddevice8->GetDeviceCaps(&caps);
	sprintf(outstr,"Screen size is %i x %i - max text = %i x %i\n",curmode.Height,curmode.Width,	caps.MaxTextureHeight,caps.MaxTextureBlendStages);
	DrawTextStr(150,100,0xff00ff00,outstr);
	sprintf(outstr,"First texture is %i x %i\n",text_desc[0].Height,text_desc[0].Width);
	DrawTextStr(150,150,0xff00ff00,outstr);
	sprintf(outstr,"Second texture is %i x %i\n",text_desc[1].Height,text_desc[1].Width);
	DrawTextStr(150,200,0xff00ff00,outstr);
	sprintf(outstr,"Third texture is %i x %i\n",text_desc[2].Height,text_desc[2].Width);
	DrawTextStr(150,250,0xff00ff00,outstr);
	delete outstr;
	*/

	char *outstr2 = new char[500];
	sprintf(outstr2,"M_X = %i and Timer = %f Mode=%i\n",m_x,fps,curmode.Format);
	DrawTextStr(150,300,D3DCOLOR_XRGB(255,0,255),outstr2);
	delete outstr2;

	m_d3ddevice8->EndScene();
  
	m_d3ddevice8->Present(NULL,NULL,NULL,NULL);
  //}
return D3D_OK;
}


int D3DObject::DrawTextStr(int x, int y, DWORD color, const TCHAR * str)
{
	HRESULT hr;

	// Get a handle for the font to use
	HFONT hFont = (HFONT)GetStockObject(SYSTEM_FONT);

	LPD3DXFONT pFont = NULL;

	// Create the D3DX Font
	hr = D3DXCreateFont(m_d3ddevice8, hFont, &pFont);
	if(FAILED(hr))
		return FALSE;

	// Rectangle where the text will be located
	RECT TextRect = { x, y, 0, 0 };

	// Inform font it is about to be used
	pFont->Begin();

	// Calculate the rectangle the text will occupy
	pFont->DrawText( str, -1, &TextRect, DT_CALCRECT, 0);

	// Outputn the text, left aligned
	pFont->DrawText( str, -1, &TextRect, DT_LEFT, color );

	// Finish up drawing
	pFont->End();
	pFont->Release();
}

void D3DObject::BlitRect(LPDIRECT3DTEXTURE8 lpSrc,
              float left, float top,
              float right, float bottom,
              D3DCOLOR col,float z)
{
    // calculate rhw
	LPDIRECT3DDEVICE8 lpDevice = m_d3ddevice8;

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
    lpDevice->SetVertexShader(D3DFVF_TLVERTEX);
    // draw the rectangle
    lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,4,verts,sizeof(D3DTLVERTEX));
}

void D3DObject::CopySurfaceToTexture(IDirect3DSurface8 *sf, IDirect3DTexture8 *tx)
{
   IDirect3DSurface8 *dst;
   tx->GetSurfaceLevel(0,&dst);
   m_d3ddevice8->CopyRects(sf,0,0,dst,0);
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

    m_pD3DXFont->Begin();
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
    m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    lstrcpy( szMsg, TEXT("Press 'Ctrl-O' to load image") );
    nNextLine -= 14; rct.top = nNextLine; rct.bottom = rct.top + 20;    
    m_pD3DXFont->DrawText( szMsg, -1, &rct, 0, fontColor );

    m_pD3DXFont->End();

    return S_OK;
}

