#pragma once

//#include "Timer.h"
//#include "Texture.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#define MAX_TEXTURES 255

class CTexture;

#define SafeRelease(x) if (x) {x->Release(); x=NULL;}
#define D3DFVF_TLVERTEX D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1

typedef struct _D3DTLVERTEX {
    float sx; /* Screen coordinates */
    float sy;
    float sz;
    float rhw; /* Reciprocal of homogeneous w */
    D3DCOLOR color; /* Vertex color */
    float tu; /* Texture coordinates */
    float tv;
    _D3DTLVERTEX() { }
    _D3DTLVERTEX(const D3DVECTOR& v, float _rhw,
                 D3DCOLOR _color, 
                 float _tu, float _tv)
    { sx = v.x; sy = v.y; sz = v.z; rhw = _rhw;
      color = _color; 
      tu = _tu; tv = _tv;
    }
} D3DTLVERTEX, *LPD3DTLVERTEX;

struct CVERTEX
{
    FLOAT  x, y, z, rhw;		// The position.
    FLOAT  u, v;			    // The texture coordinates.
};
#define D3DFVF_CVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1)

class cGameState;

class D3DObject
{
  friend class cGameState;
public:
	D3DObject(void);
	~D3DObject(void);
	int m_cnt;

	IDirect3D9* m_d3d9;
	IDirect3DDevice9* m_d3ddevice9;
//	IDirect3DSurface9* m_d3dbackbuffer9;

	D3DPRESENT_PARAMETERS m_d3dpp;
	D3DCAPS9 m_d3dcps;

	int _InitD3D9(void);
	int _InitFonts(void);

	IDirect3DSurface9* m_pBackgroundSurface;
	IDirect3DTexture9* m_pVidmemTex1;
	D3DDISPLAYMODE curmode; //current mode under consideration
  ID3DXFont*              m_pD3DXFont;            // D3DX font    
//  ID3DXFont*              m_pD3DXSmallFont;       // Smaller Font
	int m_x;
	LPD3DXSPRITE m_pd3dxSprite;

protected:
	HRESULT RenderText();

	static BOOL m_initfonts;
	LPD3DXFONT pFont;
	LPD3DXFONT pFont_StatusBar;
  IDirect3DTexture9 *m_textures[MAX_TEXTURES];
  int m_numTextures;
  void DefaultRenderState();

	//	CTimer m_timer;

public:
	BOOL D3DObject::DeviceLost();
	// Paint a frame
	int PaintFrame(IDirect3DSurface9* in_Frame);
	int MakeScreenSurface(int, int, D3DFORMAT, IDirect3DSurface9 ** );
	int LoadSurfaceFromFile(char *, IDirect3DSurface9 ** , D3DCOLOR ckey = 0xFFFF00FF,D3DXIMAGE_INFO *SrcInfo=NULL, D3DFORMAT fmt=(D3DFORMAT)NULL);
	int CopyRects(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRectsArray,UINT cRects,
				  IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPointsArray);
	int CopyBackTexture(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects,
				 CONST POINT*, int DEST);
	int LoadTextureFromFile(char *fname, IDirect3DTexture9 **texture, D3DCOLOR colorkey=0,D3DXIMAGE_INFO *SrcInfo=NULL);
	int CreateTexture(UINT Width,UINT Height,UINT  Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture);
	int PaintText ();
	int DrawTextStr(int x, int y, DWORD color, const TCHAR * str);
	int DrawTextStr_StatusBar(RECT *rct, DWORD color, const TCHAR * str, DWORD fmt);
	void BlitRect(LPDIRECT3DTEXTURE9 lpSrc,
              float left, float top,
              float right, float bottom,
              D3DCOLOR col,float z);
	void CopySurfaceToTexture(IDirect3DSurface9 *, IDirect3DTexture9 *);
	void GetTextureParms(int *max_height, int *max_width);
	int SpriteDraw( IDirect3DTexture9 *texture, RECT *rect, D3DXVECTOR2 *scale, D3DXVECTOR2 *rotate, float rotation,
						  D3DXVECTOR2 *trans, D3DCOLOR color);
	void BeginPaint();
	void EndPaint();
	int GetWidth(void) { return curmode.Width; }
	int GetHeight(void) { return curmode.Height; }
};

extern D3DObject *g_D3DObject;							// Main D3D Object