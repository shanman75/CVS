#pragma once
#define DIRECTINPUT_VERSION 0x800
#include "Timer.h"
#include <dinput.h>
#include "Camera.h"
#include "Terrain.h"

#define SafeRelease(x) if (x) {x->Release(); x=NULL;}

class D3DInput
{
public:
	D3DInput(void);
	~D3DInput(void);
/*
	void GetInput(CHero *hero);
	void GetInput(CHero2 *hero);
	void GetInput(CObjBmp *bmp);
	void GetInput(CObjMenu *mnu);
*/
	void GetInput(cCamera *cam);
	void GetInput(cTerrain *);
  BOOL KeyDown(BYTE);
  BOOL MouseDown(DWORD);
  BOOL MouseAxis(D3DXVECTOR3 *mo_axis);
  BOOL MouseScreen(D3DXVECTOR2 *mo_screen);

private:
  BOOL UpdateMouse();
	IDirectInput8 *m_DInput;
	IDirectInputDevice8 *m_DIKB;
  IDirectInputDevice8 *m_DIMO;

  CTimer m_keytime;
  CTimer m_mousetime;
  CTimer m_mouseDNtime;
  CTimer m_mouseAXtime;
  CTimer m_mouseSCtime;

  D3DXVECTOR2 m_mousepos;

  DIMOUSESTATE2 m_dims;
};

extern D3DInput *g_D3DInput;
