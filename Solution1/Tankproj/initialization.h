/****************************************************************

initialization.h

Header file for the initialization .cpp file
Last modified:April 8, 2004
****************************************************************/

#ifndef __init_h__
#define __init_h__

#include "resource.h"
#include "d3dmat.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <stdio.h>

#define SCREEN_WIDTH 640 //pixels wide
#define SCREEN_HEIGHT 480 //pixels high

struct BILLBOARDVERTEX
{ //billboard vertex
  D3DXVECTOR3 p;      //position
  float tu,tv; //texture coordinates
  static const DWORD FVF; //flexible vertex format
};

//input device type
enum InputDeviceType{KEYBOARD_INPUT,MOUSE_INPUT};

//prototypes
void InitModeStrings(D3DDISPLAYMODE*,int);
void ReleaseModeStrings(int);
LRESULT CALLBACK ModesDlgProc(HWND,UINT,WPARAM,LPARAM);
BOOL ChooseGraphicsMode(HINSTANCE,HWND,D3DDISPLAYMODE&);
HRESULT CreateD3DDevice(HWND,D3DDISPLAYMODE);
HWND CreateDefaultWindow(char*,HINSTANCE,int);
bool initd3d(HINSTANCE,HWND);
void ReleaseD3D();
BOOL RestoreDevice();
BOOL DeviceLost();

#endif