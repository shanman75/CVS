#include "StdAfx.h"
#include "d3dobject.h"
#include <stdio.h>
#include "MainWnd.h"
#include "GameState.h"

//#include "objmgr.h"
//#include "text.h"

D3DObject *g_D3DObject;							// Main D3D Object
BOOL D3DObject::m_initfonts;
//CText m_text;

D3DObject::D3DObject(void) : m_d3d9(NULL), 
                             m_d3ddevice9(NULL),
//							 m_d3dbackbuffer9(NULL),
							 m_pBackgroundSurface(NULL),
							 m_cnt(0)
{
    m_numTextures=0;
   _InitD3D9();
}

D3DObject::~D3DObject(void)
{
  OutputDebugString("D3DObject - Delete\n");
  SafeRelease(m_pd3dxSprite);
  m_d3ddevice9->ShowCursor(TRUE);
  ShowCursor(TRUE);
  SafeRelease(m_pBackgroundSurface);
  //SafeRelease(m_d3dbackbuffer9);
  SafeRelease(m_d3ddevice9);
  SafeRelease(m_d3d9);
  SafeRelease(pFont);
}

int D3DObject::_InitD3D9(void)
{
//  HRESULT retval;

  pFont = NULL;

  if ((m_d3d9=Direct3DCreate9(D3D_SDK_VERSION)) ==NULL) return FALSE;

  ZeroMemory (&m_d3dpp,sizeof(D3DPRESENT_PARAMETERS)); //null it out

  curmode.Height = 0;
  curmode.Width  = 0;
  int m = 0;

  while (!(curmode.Width == WIDTH || curmode.Height == HEIGHT))
  {
    D3DFORMAT dfmt;
	  dfmt = D3DFMT_X8R8G8B8;
	
	  if (FAILED(m_d3d9->EnumAdapterModes(D3DADAPTER_DEFAULT,dfmt,m++,&curmode)))
		exit(1);
  }

  m_d3dpp.BackBufferWidth=curmode.Width; //width
  m_d3dpp.BackBufferHeight=curmode.Height; //height
  m_d3dpp.BackBufferFormat=curmode.Format; //color mode
  m_d3dpp.BackBufferCount=1; //one back buffer
  m_d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;
  m_d3dpp.MultiSampleQuality = 0;
  m_d3dpp.SwapEffect=D3DSWAPEFFECT_FLIP; //flip pages
  m_d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD; // Try it..
//  m_d3dpp.SwapEffect             = D3DSWAPEFFECT_COPY_VSYNC;
  m_d3dpp.hDeviceWindow=NULL; //full screen
  m_d3dpp.Windowed=false; //full screen
  m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
//  m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
  m_d3dpp.EnableAutoDepthStencil=true; //not needed
  m_d3dpp.AutoDepthStencilFormat=D3DFMT_D16; //not needed
  m_d3dpp.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //can lock buffer
  m_d3dpp.Flags = 0;
  m_d3dpp.FullScreen_RefreshRateInHz =D3DPRESENT_RATE_DEFAULT;


  if (WINDOWED) {  
    m_d3dpp.Windowed = true;
    m_d3dpp.hDeviceWindow = g_hWnd;
    m_d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    m_d3dpp.BackBufferCount = 1;
    m_d3dpp.FullScreen_RefreshRateInHz = 0;
    SetWindowPos(
			  g_hWnd,
			  NULL,
			  0,
			  0,
			  800,
			  600,
			  SWP_NOZORDER | SWP_SHOWWINDOW
		  );
    m_d3dpp.BackBufferWidth=800; //width
    m_d3dpp.BackBufferHeight=600; //height
  }
  if (m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,
  			       D3DDEVTYPE_HAL,
               g_hWnd,
						   D3DCREATE_FPU_PRESERVE | D3DCREATE_HARDWARE_VERTEXPROCESSING,
						   &m_d3dpp,
						   &m_d3ddevice9
						  ) != D3D_OK)
  {
    OutputDebugString("Failed creating HAL/Hardware device\n");
    if (m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,
  			       D3DDEVTYPE_HAL,
               g_hWnd,
						   D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						   &m_d3dpp,
						   &m_d3ddevice9
               ) != D3D_OK) {
      OutputDebugString("Failed creating HAL/Software device\n");  
      if (m_d3d9->CreateDevice(D3DADAPTER_DEFAULT,
  			       D3DDEVTYPE_HAL,
               g_hWnd,
						   D3DCREATE_FPU_PRESERVE | D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						   &m_d3dpp,
						   &m_d3ddevice9
						  ) != D3D_OK)
      {
            MessageBox(NULL,"Could not create Direct3D9 Device","Unknown error creating device",MB_OK);
	          exit(1);
      }
               }
  }
  DefaultRenderState();
  m_d3ddevice9->GetDeviceCaps(&m_d3dcps);
  /*
  int immed = 0, intrvl = 0;

  if (m_d3dcps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE)
    immed = 1;
  if (m_d3dcps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE)
    intrvl = 1;
  if (m_d3dcps.PresentationIntervals & D3DPRESENT_INTERVAL_TWO)
    intrvl = 2;
  if (m_d3dcps.PresentationIntervals & D3DPRESENT_INTERVAL_THREE)
    intrvl = 3;
  if (m_d3dcps.PresentationIntervals & D3DPRESENT_INTERVAL_FOUR)
    intrvl = 4;
  */

  D3DXCreateSprite(m_d3ddevice9,&m_pd3dxSprite);
  _InitFonts();

  char outstr[5000];
  sprintf (outstr,"Device Statistics\nMaxTexHeight: %i, MaxTexWidth: %i\n",m_d3dcps.MaxTextureHeight,m_d3dcps.MaxTextureWidth);
  OutputDebugString(outstr);
  return TRUE;
}

void D3DObject::DefaultRenderState()
{
  m_d3ddevice9->SetRenderState ( D3DRS_ZENABLE, D3DZB_TRUE);
  m_d3ddevice9->SetRenderState ( D3DRS_ZWRITEENABLE, TRUE);
  m_d3ddevice9->SetRenderState ( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

  m_d3ddevice9->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
  m_d3ddevice9->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
  m_d3ddevice9->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

  float fogNear = 100.0f;
  float fogFar = 400.0f;
  float fogDense =0.2f;

  /*
m_d3ddevice9->SetRenderState(D3DRS_FOGENABLE, 1);
m_d3ddevice9->SetRenderState(D3DRS_FOGCOLOR,  D3DCOLOR_RGBA(40,40,40,255));
//m_d3ddevice9->SetRenderState(D3DRS_FOGSTART, 0);
m_d3ddevice9->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&fogDense));
m_d3ddevice9->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fogNear));
m_d3ddevice9->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fogFar));
m_d3ddevice9->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR );
 */

  m_d3ddevice9->SetRenderState ( D3DRS_AMBIENT, D3DCOLOR_RGBA(190,150,190,255));

  m_d3ddevice9->SetRenderState ( D3DRS_CULLMODE, D3DCULL_CCW);
  //m_d3ddevice9->SetRenderState ( D3DRS_LIGHTING, FALSE);
/*
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_ALPHAOP,  D3DTOP_SELECTARG1);
*/

  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );

//  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
//  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
//  m_d3ddevice9->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
  m_d3ddevice9->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
  m_d3ddevice9->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
  
  //  m_d3ddevice9->SetVertexShader( D3DFVF_CVERTEX );


  //if (m_d3ddevice9->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&m_d3dbackbuffer9) != D3D_OK) {
  //  m_d3dbackbuffer9=NULL; return FALSE;
  //}

  D3DLIGHT9 light,light2,light3;
  ZeroMemory(&light,sizeof(light));
  D3DXVECTOR3 pos (0,400,0);
  //g_D3DObject->m_d3ddevice9->GetLight(0,&light);
  light.Position = pos;
  light.Direction = D3DXVECTOR3 (0,-1,0);
  light.Diffuse.g = light.Diffuse.b =   light.Diffuse.r = 1.0f;
  light.Specular = light.Diffuse;
  light.Diffuse.r = 0.9f;
  light.Range = 5000;
  light.Type = D3DLIGHT_DIRECTIONAL;
  m_d3ddevice9->SetLight(0,&light);
  m_d3ddevice9->LightEnable(0,true);

  light2 = light;
  light3 = light;
  light2.Position  = D3DXVECTOR3( 30.0f,400.0f,30.0f);
  light3.Position  = D3DXVECTOR3(-30.0f,400.0f,30.0f);
  light2.Direction = D3DXVECTOR3(  0.2f, -1.0f, 0.2f);
  light3.Direction = D3DXVECTOR3( -0.2f, -1.0f,-0.2f);

  //m_d3ddevice9->SetLight(1,&light2);
  //m_d3ddevice9->LightEnable(1,true);
  //m_d3ddevice9->SetLight(2,&light3);
  //m_d3ddevice9->LightEnable(2,true);

  ShowCursor(FALSE);
  m_d3ddevice9->ShowCursor(FALSE);
}

// Paint a frame
int D3DObject::PaintFrame(IDirect3DSurface9* in_Frame)
{
	DeviceLost();

	m_d3ddevice9->BeginScene();
	//m_d3ddevice9->UpdateSurface(in_Frame,NULL,m_d3dbackbuffer9,NULL);
	//m_d3ddevice9->CopyRects(in_Frame,NULL,0,m_d3dbackbuffer8,NULL);
	m_d3ddevice9->EndScene();
	m_d3ddevice9->Present(NULL,NULL,NULL,NULL);
	return 0;
}

int D3DObject::MakeScreenSurface(int width, int height, D3DFORMAT format, IDirect3DSurface9** dest_surf)
{
	HRESULT bl;

	if (width == 0) width = m_d3dpp.BackBufferWidth;
	if (height == 0) height = m_d3dpp.BackBufferHeight;
	if (format == D3DFMT_UNKNOWN) format = m_d3dpp.BackBufferFormat;

	bl = m_d3ddevice9->CreateOffscreenPlainSurface(
		width,
		height,
		format,
		D3DPOOL_SYSTEMMEM,
		dest_surf,
		NULL
  );

	if (FAILED(bl))
		exit(1);
	return bl;

	//return m_d3ddevice9->CreateImageSurface(width,height,
	//	format,dest_surf);
}

BOOL D3DObject::DeviceLost(){ //check for lost device
  if (m_d3ddevice9->TestCooperativeLevel()!=D3D_OK)
  {
    while (m_d3ddevice9->TestCooperativeLevel()==D3DERR_DEVICELOST)
      ;
	  OutputDebugString("D3DObject::DeviceLost Restoring Surfaces\n");
    m_pd3dxSprite->OnLostDevice();
    g_GameState->OnLostDevice();
	  OutputDebugString("D3DObject::DeviceLost Restoring Font\n");
    pFont->OnLostDevice();
    pFont_StatusBar->OnLostDevice();
    switch(m_d3ddevice9->Reset(&m_d3dpp)) {
        case D3DERR_DEVICELOST:
          OutputDebugString ("D3DERR_DEVICELOST\n");exit(1); break;
        case D3DERR_DRIVERINTERNALERROR:
          OutputDebugString("D3DERR_DRIVERINTERNALERROR\n"); exit(1); break;
        case D3DERR_INVALIDCALL:
          OutputDebugString("D3DERR_INVALIDCALL\n"); exit(1); break;
        case D3DERR_OUTOFVIDEOMEMORY:
          OutputDebugString("D3DERR_OUTOFVIDEOMEMORY\n");exit(1); break;
        case E_OUTOFMEMORY:
          OutputDebugString("E_OUTOFMEMORY\n");exit(1); break;
        default:
          OutputDebugString("Unknown Error\n"); break;
    }
    DefaultRenderState();
    m_pd3dxSprite->OnResetDevice();
    pFont->OnResetDevice();
    pFont_StatusBar->OnResetDevice();
    g_GameState->OnResetDevice();
    return false;
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

	pFont = NULL;

	// Create the D3DX Font
	D3DXFONT_DESC fdesc;
	memset (&fdesc,0,sizeof(fdesc));
	strcpy (fdesc.FaceName,"Arial");
	fdesc.Weight = FW_NORMAL;
	fdesc.Height = 32;
	if (FAILED(D3DXCreateFontIndirect(m_d3ddevice9, &fdesc, &pFont)))
  {
     MessageBox(NULL,"TankGWAR Error","Failed to create font!",MB_OK);
     exit(1);
  }

	strcpy (fdesc.FaceName,"Arial");
	fdesc.Weight = FW_BOLD;
	fdesc.Height = 27;
  if (FAILED(D3DXCreateFontIndirect(m_d3ddevice9, &fdesc, &pFont_StatusBar)))
  {
     MessageBox(NULL,"TankGWAR Error","Failed to create font!",MB_OK);
     exit(1);
  }
  
  m_initfonts=TRUE;
	return m_initfonts;
}

int D3DObject::DrawTextStr(int x, int y, DWORD color, const TCHAR * str)
{

	if (!m_initfonts) _InitFonts();
	// Rectangle where the text will be located
	RECT TextRect = { x, y, 0, 0 };

	// Inform font it is about to be used
	//pFont->Begin();

	// Calculate the rectangle the text will occupy
	pFont->DrawText( NULL, str, -1, &TextRect, DT_CALCRECT, color);

	// Outputn the text, left aligned
	pFont->DrawText( NULL, str, -1, &TextRect, DT_LEFT | DT_NOCLIP, color );

	// Finish up drawing
	//pFont->End();
	return TRUE;
}

//int D3DObject::DrawTextStr_StatusBar(int x, int y, DWORD color, const TCHAR * str)
int D3DObject::DrawTextStr_StatusBar(RECT *rct, DWORD color, const TCHAR * str, DWORD fmt)
{

	if (!m_initfonts) _InitFonts();
	// Rectangle where the text will be located
//	RECT TextRect = { x, y, 0, 0 };

	// Inform font it is about to be used
	//pFont->Begin();

	// Calculate the rectangle the text will occupy
	//pFont_StatusBar->DrawText( NULL, str, -1, &TextRect, DT_CALCRECT, color);

	// Outputn the text, left aligned
	pFont_StatusBar->DrawText( NULL, str, -1, rct, fmt, color );

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
   D3DXMATRIXA16 transform;
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
  m_d3ddevice9->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
          D3DCOLOR_XRGB(0,0,255), 1.0f, 0L );
	m_d3ddevice9->BeginScene();
//	m_pd3dxSprite->Begin(D3DXSPRITE_DONOTSAVESTATE|D3DXSPRITE_OBJECTSPACE);
//	m_pd3dxSprite->Begin(0);
}

void D3DObject::EndPaint()
{	
//	m_pd3dxSprite->End();
	m_d3ddevice9->EndScene();  
	m_d3ddevice9->Present(NULL,NULL,NULL,NULL);
}