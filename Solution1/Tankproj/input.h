/***************************************************************
input.h

Header file for the input file

last modified: march 29, 2004
***************************************************************/

#ifndef __INPUT_H__
#define __INPUT_H__

#include <dinput.h>
#include <stdio.h>
#include "initialization.h"

#define DI_BUFSIZE 16 //buffer for DirectInput events from devices
//#define DIRECTINPUT_VERSION 0x0800 //put before #include dinput.h

class CInputManager{
  private:
    LPDIRECTINPUT8 lpDirectInput; //DirectInput object
    LPDIRECTINPUTDEVICE8 pKeyboard; //keyboard device
    DIDEVICEOBJECTDATA DIod[DI_BUFSIZE]; //keyboard buffer
    LPDIRECTINPUTDEVICE8 pMouse; //mouse device
    HANDLE hMouseEvent; //mouse interrupt event
    POINT pointMouse; //mouse point
    InputDeviceType nInputDevice; //current input device
	//private helper functions
    BOOL KeyboardHandler(DWORD keystroke); //keyboard handler
    void GameKeyboardHandler(DWORD keystroke,BOOL override=FALSE); //game phase
    void SetPlaneSpeed(POINT point,SIZE extent); //plane speed from mouse
    //for text input
    BOOL letter(WPARAM keystroke,char& ascii); //keystroke is a letter
    BOOL number(WPARAM keystroke,char& ascii); //keystroke is a digit
    //setting of input device from registry
    void InitInputDevice(); //get last used device
    void SetInputDevice(); //put last used device
    //setup functions
    BOOL InitKeyboardInput(HWND hwnd); //set up keyboard input
    BOOL InitMouseInput(HWND hwnd); //set up mouse input
    //mouse handlers
    void MouseMove(int x,int y); //handle mouse motion
    //left mouse button down handlers
    void GameLMouseDown(); //mouse down handler 
    //left mouse button up handlers
    void LMouseDown(); //main mouse down handler
    BOOL LMouseUp(); //main mouse up handler
  public:
    CInputManager(HINSTANCE hInstance,HWND hwnd); //constructor
    ~CInputManager(); //destructor
    BOOL ProcessKeyboardInput(); //process buffered keyboard events
    BOOL ProcessMouseInput(); //process buffered mouse events
};
#endif
