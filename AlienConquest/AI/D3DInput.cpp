#include "StdAfx.h"
#include "d3dinput.h"

D3DInput *g_D3DInput;

D3DInput::D3DInput(void)
{	
	if (FAILED( DirectInput8Create(hInst, DIRECTINPUT_VERSION, 
        IID_IDirectInput8, (void**)&m_DInput, NULL))) 
		return;
	if (FAILED( m_DInput->CreateDevice(GUID_SysKeyboard, &m_DIKB, NULL)))
		return; 
	if (FAILED( m_DIKB->SetDataFormat(&c_dfDIKeyboard)))
		return;
	if (FAILED( m_DIKB->SetCooperativeLevel(hwnd, 
                            DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return;

	  DIPROPDWORD dipdw;
  dipdw.diph.dwSize=sizeof(DIPROPDWORD);
  dipdw.diph.dwHeaderSize=sizeof(DIPROPHEADER);
  dipdw.diph.dwObj=0;
  dipdw.diph.dwHow=DIPH_DEVICE;
  dipdw.dwData=1; //buffer size
    if(FAILED(m_DIKB->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph)))
		return;
	if (FAILED( m_DIKB->Acquire()))
		return;

}

void D3DInput::GetInput(CHero *hero) 
{ 
    #define KEYDOWN(name, key) (name[key] & 0x80) 
 
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
	if(KEYDOWN(buffer, DIK_S)){
//		hero->SetSpeed(0,0);
//		hero->SetAccel(0,0);
	}

	if (KEYDOWN(buffer, DIK_SPACE)) {
		hero->event(CHero::FIRE);
	}

	if (KEYDOWN(buffer, DIK_PAUSE)) {
		OutputDebugString("Pause\n");
//		CTimer t_time;
//		t_time.ToggleTimer();
	}
} 

D3DInput::~D3DInput(void)
{
	if (m_DIKB) m_DIKB->Unacquire();
	SafeRelease(m_DIKB);
	SafeRelease(m_DInput);
}
