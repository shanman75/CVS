/***************************************************************
input.h

File for the input in the program which will be directinput

last modified: march 29, 2004
***************************************************************/

#include "input.h"

CInputManager::CInputManager(HINSTANCE hInstance,HWND hwnd){ //constructor
  nInputDevice=KEYBOARD_INPUT; //default to keyboard input
  InitInputDevice(); //current input device
  //create DirectInput object
  HRESULT hr=DirectInput8Create(hInstance,DIRECTINPUT_VERSION, 
    IID_IDirectInput8A,(void**)&lpDirectInput,NULL); 
  if(FAILED(hr)){
    lpDirectInput=NULL; //in case it fails
    return; //bail out
  }
  InitKeyboardInput(hwnd); //set up keyboard
  InitMouseInput(hwnd); //set up mouse

}

CInputManager::~CInputManager(){ //destructor
  if(pKeyboard){ //release keyboard
    pKeyboard->Unacquire(); //unacquire device
    pKeyboard->Release(); //release it
  }
  if(pMouse){ //release mouse
    pMouse->Unacquire(); //unacquire device
    pMouse->Release(); //release it
  }

  //now release DirectInput
  if(lpDirectInput)lpDirectInput->Release();
}

//registry access for current input device

void CInputManager::InitInputDevice(){ 
//get last used device from registry
  HKEY RegistryKey; //registry key
  const int BUFSIZE=8; //buffer size
  char buffer[BUFSIZE]; //text buffer
  int result=RegCreateKey(HKEY_CURRENT_USER,
    "SOFTWARE\\LARC\\NedFarm",&RegistryKey);
  DWORD type; //type of registry entry
  if(result==ERROR_SUCCESS){ //entry exists
    unsigned long length=BUFSIZE-1; buffer[0]=0;
    RegQueryValueEx(RegistryKey,"InputDevice",NULL,&type,
       (unsigned char *)buffer,&length); //get value into buffer
    if(length){ //success
      buffer[length]=0; sscanf(buffer,"%d",&nInputDevice);
    }
    else nInputDevice=KEYBOARD_INPUT; //default to keyboard input
    RegCloseKey(RegistryKey);
  }
}

void CInputManager::SetInputDevice(){ 
//save last used device to registry
  HKEY RegistryKey; //registry key
  if(RegCreateKey(HKEY_CURRENT_USER,"SOFTWARE\\LARC\\NedFarm",
    &RegistryKey)==ERROR_SUCCESS){ //entry exists
    const int BUFSIZE=16; //buffer size
    char buffer[BUFSIZE]; //text buffer
    sprintf(buffer,"%d",nInputDevice); //convert to string
    RegSetValueEx(RegistryKey,"InputDevice",NULL,REG_SZ,
      (unsigned char *)buffer,(DWORD)strlen(buffer)+1); //set value
    RegCloseKey(RegistryKey);
  }
}

//DirectInput setup routines

BOOL CInputManager::InitKeyboardInput(HWND hwnd){ //set up keyboard input
  //create keyboard device
  HRESULT hr=lpDirectInput->CreateDevice(GUID_SysKeyboard,&pKeyboard,NULL); 
  if(FAILED(hr)){
    pKeyboard=NULL; //in case it fails
    return FALSE; //bail out
  }
  //set keyboard data format, c_dfDIKeyboard is a "predefined global"
  //yucko ugly part of DirectInput, thanks a lot Microsoft
  hr=pKeyboard->SetDataFormat(&c_dfDIKeyboard); 
  if(FAILED(hr))return FALSE; //fail and bail
  //set the cooperative level 
  hr=pKeyboard->SetCooperativeLevel(hwnd, 
    DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);  
  if(FAILED(hr))return FALSE; //fail and bail
  //set buffer size
  DIPROPDWORD dipdw;
  dipdw.diph.dwSize=sizeof(DIPROPDWORD);
  dipdw.diph.dwHeaderSize=sizeof(DIPROPHEADER);
  dipdw.diph.dwObj=0;
  dipdw.diph.dwHow=DIPH_DEVICE;
  dipdw.dwData=DI_BUFSIZE; //buffer size
  hr=pKeyboard->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph);  
  if(FAILED(hr))return FALSE; //fail and bail
  //acquire the keyboard
  hr=pKeyboard->Acquire();  
  if(FAILED(hr))return FALSE; //fail and bail
  return TRUE;
}

BOOL CInputManager::InitMouseInput(HWND hwnd){ //set up mouse input
  //create mouse device 
  HRESULT hr=lpDirectInput->CreateDevice(GUID_SysMouse,&pMouse,NULL);
    if(FAILED(hr)){
    pMouse=NULL; //in case it fails
    return FALSE; //bail out
  }
  //set mouse data format
  hr=pMouse->SetDataFormat(&c_dfDIMouse);
  if(FAILED(hr))return FALSE;  //fail and bail
  //set the cooperative level
  hr=pMouse->SetCooperativeLevel(hwnd,
    DISCL_EXCLUSIVE|DISCL_FOREGROUND);
  if(FAILED(hr))return FALSE; //fail and bail
  //set up mouse event
  hMouseEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
  if(!hMouseEvent)return FALSE; //fail and bail
  hr=pMouse->SetEventNotification(hMouseEvent);
  if(FAILED(hr))return FALSE; //fail and bail
   //set buffer size
  DIPROPDWORD dipdw;
  dipdw.diph.dwSize=sizeof(DIPROPDWORD);
  dipdw.diph.dwHeaderSize=sizeof(DIPROPHEADER);
  dipdw.diph.dwObj=0;
  dipdw.diph.dwHow=DIPH_DEVICE;
  dipdw.dwData=DI_BUFSIZE; //buffer size
  hr=pMouse->SetProperty(DIPROP_BUFFERSIZE,&dipdw.diph);  
  if(FAILED(hr))return FALSE; //fail and bail
  //acquire the mouse
  hr=pMouse->Acquire();  
  if(FAILED(hr))return FALSE; //fail and bail
  return TRUE;
}

//next 2 globals are a kluge for callback functions
LPDIRECTINPUT8 g_lpDirectInput; //DirectInput object

//keyboard processing
BOOL CInputManager::ProcessKeyboardInput(){ //process buffered keyboard events
  BOOL result=FALSE; //return result
  if(!pKeyboard)return FALSE; //no device so fail and bail
  DWORD dwElements=DI_BUFSIZE; //number of events to process
  //get number of events to process into member buffer DIod
  HRESULT hr=pKeyboard->GetDeviceData(
    sizeof(DIDEVICEOBJECTDATA),DIod,&dwElements,0);
  //if it failed, we may be able to acquire the device
  if(FAILED(hr)){ //lost the device somehow
    hr=pKeyboard->Acquire(); //try to reacquire
    while(hr==DIERR_INPUTLOST)hr=pKeyboard->Acquire(); //try real hard
    return FALSE; //live to fight another day
  }
  if(FAILED(hr))return FALSE; //fail and bail paranoia
  //process inputs from DIod now
  for(int i=0; (i<(int)dwElements)&&!result; i++){ //for each input
    if(DIod[i].dwData&0x80) //if key went down
      result=result||KeyboardHandler(DIod[i].dwOfs); //handle it
  }
  return result;
}

//keyboard handlers for different phases

BOOL CInputManager::KeyboardHandler(DWORD keystroke)
{ //main keyboard handler
  BOOL result=FALSE; //TRUE if we are to exit game
  switch(keystroke)
  {	 DIK_ESC : result=TRUE;
               break;
  }
  return result;
} //KeyboardHandler


BOOL CInputManager::letter(WPARAM keystroke,char& ascii){
  BOOL result=FALSE;
  switch(keystroke){
    case DIK_A: result=TRUE; ascii='a'; break;
    case DIK_B: result=TRUE; ascii='b'; break;
    case DIK_C: result=TRUE; ascii='c'; break;
    case DIK_D: result=TRUE; ascii='d'; break;
    case DIK_E: result=TRUE; ascii='e'; break;
    case DIK_F: result=TRUE; ascii='f'; break;
    case DIK_G: result=TRUE; ascii='g'; break;
    case DIK_H: result=TRUE; ascii='h'; break;
    case DIK_I: result=TRUE; ascii='i'; break;
    case DIK_J: result=TRUE; ascii='j'; break;
    case DIK_K: result=TRUE; ascii='k'; break;
    case DIK_L: result=TRUE; ascii='l'; break;
    case DIK_M: result=TRUE; ascii='m'; break;
    case DIK_N: result=TRUE; ascii='n'; break;
    case DIK_O: result=TRUE; ascii='o'; break;
    case DIK_P: result=TRUE; ascii='p'; break;
    case DIK_Q: result=TRUE; ascii='q'; break;
    case DIK_R: result=TRUE; ascii='r'; break;
    case DIK_S: result=TRUE; ascii='s'; break;
    case DIK_T: result=TRUE; ascii='t'; break;
    case DIK_U: result=TRUE; ascii='u'; break;
    case DIK_V: result=TRUE; ascii='v'; break;
    case DIK_W: result=TRUE; ascii='w'; break;
    case DIK_X: result=TRUE; ascii='x'; break;
    case DIK_Y: result=TRUE; ascii='y'; break;
    case DIK_Z: result=TRUE; ascii='z'; break;
  }
  return result;
}

BOOL CInputManager::number(WPARAM keystroke,char& ascii){
  BOOL result=FALSE;
  switch(keystroke){
    case DIK_0: result=TRUE; ascii='0'; break;
    case DIK_1: result=TRUE; ascii='1'; break;
    case DIK_2: result=TRUE; ascii='2'; break;
    case DIK_3: result=TRUE; ascii='3'; break;
    case DIK_4: result=TRUE; ascii='4'; break;
    case DIK_5: result=TRUE; ascii='5'; break;
    case DIK_6: result=TRUE; ascii='6'; break;
    case DIK_7: result=TRUE; ascii='7'; break;
    case DIK_8: result=TRUE; ascii='8'; break;
    case DIK_9: result=TRUE; ascii='9'; break;
  }
  return result;
}


void CInputManager::GameKeyboardHandler(DWORD keystroke,BOOL override){ //game phase
  //function keys work all the time
  switch(keystroke){
    case DIK_ESCAPE:  break;
    case DIK_F1:break;
    case DIK_F2:  return;
  }
  //other keys in KEYBOARD_INPUT mode or when overridden
  if(nInputDevice!=KEYBOARD_INPUT&&!override)return;
  switch(keystroke){
    case DIK_UP:break;
    case DIK_DOWN: break;
    case DIK_LEFT: break;
    case DIK_RIGHT:  break;
    case DIK_SPACE:  break;
    default: break;
  }
} //GameKeyboardHandler

//mouse processing

BOOL CInputManager::ProcessMouseInput(){ //process buffered mouse events
  BOOL result=FALSE; //TRUE if game is over
  if(!pMouse)return FALSE; //no device so fail and bail
  DWORD dwElements=DI_BUFSIZE; //number of events to process
  //get number of events to process into member buffer DIod
  HRESULT hr=pMouse->GetDeviceData(
    sizeof(DIDEVICEOBJECTDATA),DIod,&dwElements,0);
  //if it failed, we may be able to acquire the device
  if(FAILED(hr)){ //lost the device somehow
    hr=pMouse->Acquire(); //try to reacquire
    while(hr==DIERR_INPUTLOST)hr=pMouse->Acquire(); //try real hard
    return FALSE; //live to fight another day
  }
  if(FAILED(hr))return FALSE; //fail and bail (paranoia)
  //process inputs from DIod now
  for(int i=0; i<(int)dwElements; i++){ //for each input
    switch(DIod[i].dwOfs){
      case DIMOFS_BUTTON0: //left mouse button
        if(DIod[i].dwData&0x80)LMouseDown(); //down
        else result=LMouseUp(); //up
        break;
      case DIMOFS_X: MouseMove(DIod[i].dwData,0); break;
      case DIMOFS_Y: MouseMove(0,DIod[i].dwData); break;
    }
  }
  return result;
}

//mouse handlers

void CInputManager::SetPlaneSpeed(POINT point,SIZE extent){
//set plane speed based on point's position in extent
  int xmin=0,xmax=1024,ymin=0,ymax=768; //plane speed limits

  //bands for speed assignment
  const int XBANDWIDTH=extent.cx/(xmax-xmin+1);
  const int YBANDWIDTH=extent.cy/(ymax-ymin+1);
  int xspeed,yspeed; //speed of plane
  xspeed=point.x/XBANDWIDTH+xmin; //horizontal speed
  yspeed=point.y/YBANDWIDTH+ymin; //vertical speed

}

void CInputManager::MouseMove(int x,int y){
  //process mouse move
  pointMouse.x+=x; pointMouse.y+=y;
  //mouse as joystick
  if(nInputDevice!=MOUSE_INPUT)return; //bail if mouse not selected
  //set extent
  SIZE extent; //extent that mouse moves in
  extent.cx=SCREEN_WIDTH;
  extent.cy=SCREEN_HEIGHT; 
  //set plane speed based on point and extent
  SetPlaneSpeed(pointMouse,extent); 
}



void CInputManager::GameLMouseDown(){
//mouse down handler for game
  if(nInputDevice!=MOUSE_INPUT)return; //bail if mouse not selected
  GameKeyboardHandler(DIK_SPACE,TRUE);
}

void CInputManager::LMouseDown()
//main mouse left button down handler
{
}

//mouse left button up handlers
BOOL CInputManager::LMouseUp()
{ 
//main mouse left button up handler
  BOOL result=FALSE;
  return result;
}

