#include "StdAfx.h"
#include "d3dinput.h"
#include "MainWnd.h"

D3DInput *g_D3DInput;
#define KEYDOWN(name, key) (name[key] & 0x80) 

D3DInput::D3DInput(void)
{
  m_keytime.Reset();
	if (FAILED( DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, 
              IID_IDirectInput8, (VOID**)&m_DInput, NULL))) 
		return;

	if (FAILED( m_DInput->CreateDevice(GUID_SysKeyboard, &m_DIKB, NULL)))
		return; 
	if (FAILED( m_DIKB->SetDataFormat(&c_dfDIKeyboard)))
		return;
	if (FAILED( m_DIKB->SetCooperativeLevel(g_hWnd, 
                            DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY)))
		return;

/*
  DIPROPDWORD dipdw;
  dipdw.diph.dwSize=sizeof(DIPROPDWORD);
  dipdw.diph.dwHeaderSize=sizeof(DIPROPHEADER);
  dipdw.diph.dwObj=0;
  dipdw.diph.dwHow=DIPH_DEVICE;
  dipdw.dwData=1; //buffer size

  if(FAILED(m_DIKB->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph)))
		return;
*/

  if(FAILED(m_DInput->CreateDevice(GUID_SysMouse, &m_DIMO, NULL)))
    return;
  if(FAILED(m_DIMO->SetDataFormat(&c_dfDIMouse)))
    return;
  if(FAILED(m_DIMO->SetCooperativeLevel(g_hWnd,DISCL_EXCLUSIVE|DISCL_FOREGROUND)))
    return;

}

BOOL D3DInput::MouseDown(DWORD button)
{
  if (button > 3) return false;

  if (m_mousetime.GetTime() > 0)
    if (m_DIMO->GetDeviceState(sizeof(m_dims),(LPVOID)&m_dims) != DI_OK)
    { 
		  while(m_DIMO->Acquire() == DIERR_INPUTLOST)
		    ;
      OutputDebugString("Tried to re-acquire mouse\n");
		  return false;
    }
    /*
    if (dims.lX) {
      OutputDebugString("Button down left\n");
      cam->event(cCamera::LEFT);
    }
    if (dims.rgbButtons[0] & 0x80)
    {
      OutputDebugString("Button down left\n");
      cam->event(cCamera::LEFT);
    }
    if (dims.rgbButtons[1] & 0x80)
    {
      OutputDebugString("Button down right\n");
      cam->event(cCamera::RIGHT);
    }
    */
    if (m_dims.rgbButtons[button] & 0x80)
      return true;
    return false;
}

BOOL D3DInput::KeyDown(BYTE check)
{
    static char buffer[256]; 
    HRESULT  hr; 
 
    if (m_keytime.GetTime() > 0) {

      if (FAILED(hr = m_DIKB->GetDeviceState(sizeof(buffer),(LPVOID)&buffer))) {
 		    hr=m_DIKB->Acquire();
        while(hr==DIERR_INPUTLOST)hr=m_DIKB->Acquire(); //try real hard 
        OutputDebugString("Tried to re-acquire keyboard\n");
		    if (FAILED(hr)) return FALSE;
        m_DIKB->GetDeviceState(sizeof(buffer),(LPVOID)&buffer);
      } 
    }

    return KEYDOWN(buffer,check);
}
void D3DInput::GetInput(cCamera *cam) 
{  
    if (KeyDown(DIK_LEFT))
  		cam->event(cCamera::LEFT);
  	else if(KeyDown(DIK_RIGHT))
	  	cam->event(cCamera::RIGHT);

    if (KeyDown(DIK_UP))
		  cam->event(cCamera::UP);
	  else if(KeyDown(DIK_DOWN))
		  cam->event(cCamera::DOWN);

    if (KeyDown(DIK_A))
		  cam->event(cCamera::ZOOMIN);
	  else if(KeyDown(DIK_Z))
		  cam->event(cCamera::ZOOMOUT);

}
void D3DInput::GetInput(cTerrain *ter)
{

	static int keyup = 0;

  if (!KeyDown(DIK_R)) {
		keyup = DIK_R;
  }

	if (KeyDown(DIK_R) && keyup == DIK_R) {
		ter->event(cTerrain::RAND);
		keyup  = 0;
	}
}


/*
void D3DInput::GetInput(CObjMenu *mnu)
{
    char     buffer[256]; 
    HRESULT  hr; 
 
	static int keydwn = 0;

    hr = m_DIKB->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
    if FAILED(hr) 
    { 
		hr=m_DIKB->Acquire();
		while(hr==DIERR_INPUTLOST)hr=m_DIKB->Acquire(); //try real hard
		return;
    } 

	if (KEYDOWN(buffer, DIK_DOWN)) {
		keydwn = DIK_DOWN;
//		mnu->event(CObjMenu::DOWN);
	}
	else if (KEYDOWN(buffer, DIK_UP)) {
		keydwn = DIK_UP;
//		mnu->event(CObjMenu::UP);
	}
	else if (KEYDOWN(buffer, DIK_RETURN)) {
		keydwn = DIK_RETURN;
//		mnu->event(CObjMenu::UP);
	}

	if (KEYDOWN(buffer, DIK_ESCAPE)) {
		mnu->event(CObjMenu::ESC);
	}

	if (!KEYDOWN(buffer, DIK_DOWN) && keydwn == DIK_DOWN) {
		mnu->event(CObjMenu::DOWN);
		keydwn = 0;
	}
	else if (!KEYDOWN(buffer, DIK_UP) && keydwn == DIK_UP) {
		mnu->event(CObjMenu::UP);
		keydwn = 0;
	}
	else if (!KEYDOWN(buffer, DIK_RETURN) && keydwn == DIK_RETURN) {
		mnu->event(CObjMenu::ENTER);
		keydwn = 0;
	}

}


void D3DInput::GetInput(CObjBmp *bmp)
{
    char     buffer[256]; 
    HRESULT  hr; 
 
    hr = m_DIKB->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
    if FAILED(hr) 
    { 
		hr=m_DIKB->Acquire();
		while(hr==DIERR_INPUTLOST)hr=m_DIKB->Acquire(); //try real hard
		return;
    } 
 
	if (KEYDOWN(buffer,DIK_ESCAPE)) {
		bmp->event(CObjBmp::FIRE);
	}
	else for (int x=1; x < 255; x++) 
		if (KEYDOWN(buffer,x)) {
			bmp->event(CObjBmp::ANY);
			break;
		}
}

void D3DInput::GetInput(CHero *hero) 
{ 
 
    char     buffer[256]; 
    HRESULT  hr; 
 
    hr = m_DIKB->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
    if FAILED(hr) 
    { 
		hr=m_DIKB->Acquire();
		while(hr==DIERR_INPUTLOST)hr=m_DIKB->Acquire(); //try real hard
		return;
    } 
 
    if (KEYDOWN(buffer, DIK_RIGHT))
		hero->event(CHero::RIGHT);
    else if(KEYDOWN(buffer, DIK_LEFT))
		hero->event(CHero::LEFT);
 
    if (KEYDOWN(buffer, DIK_UP))
		hero->event(CHero::UP);
    else if (KEYDOWN(buffer, DIK_DOWN))
		hero->event(CHero::DOWN);
//	if(KEYDOWN(buffer, DIK_S)){
//		hero->SetSpeed(0,0);
//		hero->SetAccel(0,0);
//	}

	if (KEYDOWN(buffer, DIK_0))
		hero->event(CHero::GOD);

	if (KEYDOWN(buffer, DIK_SPACE)) {
		hero->event(CHero::FIRE);
	}

	static int pausek = 0;

	if (KEYDOWN(buffer, DIK_PAUSE) || KEYDOWN(buffer, DIK_P)) {
		OutputDebugString("Pause\n");
		CTimer t_time;
		if (!pausek)
			t_time.TogglePause();
//		CTimer t_time;
//		t_time.ToggleTimer();
		pausek = 1;
	}
	else
		pausek = 0;
}

void D3DInput::GetInput(CHero2 *hero) 
{ 
 
    char     buffer[256]; 
    HRESULT  hr; 
 
    hr = m_DIKB->GetDeviceState(sizeof(buffer),(LPVOID)&buffer); 
    if FAILED(hr) 
    { 
		hr=m_DIKB->Acquire();
		while(hr==DIERR_INPUTLOST)hr=m_DIKB->Acquire(); //try real hard
		return;
    } 
 
    if (KEYDOWN(buffer, DIK_D))
		hero->event(CHero2::RIGHT);
    else if(KEYDOWN(buffer, DIK_S))
		hero->event(CHero2::LEFT);
 
    if (KEYDOWN(buffer, DIK_A))
		hero->event(CHero2::UP);
    else if (KEYDOWN(buffer, DIK_Z))
		hero->event(CHero2::DOWN);
//	if(KEYDOWN(buffer, DIK_X)){
//		hero->SetSpeed(0,0);
//		hero->SetAccel(0,0);
//	}

	if (KEYDOWN(buffer, DIK_1))
		hero->event(CHero2::GOD);

	if (KEYDOWN(buffer, DIK_F)) {
		hero->event(CHero2::FIRE);
	}
}
*/
//}

D3DInput::~D3DInput(void)
{
  OutputDebugString("Tearing down D3DInput8\n");
	if (m_DIKB) m_DIKB->Unacquire();
	SafeRelease(m_DIKB);
	if (m_DIMO) m_DIMO->Unacquire();
  SafeRelease(m_DIMO);
	SafeRelease(m_DInput);
}
