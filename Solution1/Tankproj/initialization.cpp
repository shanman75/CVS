/****************************************************************

initialization.cpp

This file for the initializing the graphics hardware for direct
3d.

Last modified: March 18, 2004
****************************************************************/

#include "initialization.h"

extern IDirect3D9 *d3dcomobj;
extern IDirect3DDevice9 *d3ddevice;
extern D3DPRESENT_PARAMETERS presparams;
extern LPDIRECT3DVERTEXBUFFER9 sbnegx;
extern LPDIRECT3DVERTEXBUFFER9 sbposx;
extern LPDIRECT3DVERTEXBUFFER9 sbnegz;
extern LPDIRECT3DVERTEXBUFFER9 sbposz;
extern LPDIRECT3DVERTEXBUFFER9 sbtop;

char** g_pszGraphicsModeString=NULL; //human-readable strings for graphics modes
const int MODESTRINGLENGTH=64; //max length of string
int g_nNumAcceptableModes=0; //number of acceptable graphics modes
const DWORD BILLBOARDVERTEX::FVF=D3DFVF_XYZ|D3DFVF_TEX1; //flexible vertex format

//prototypes
LRESULT CALLBACK WindowProc(HWND,UINT, WPARAM,LPARAM);

void InitModeStrings(D3DDISPLAYMODE* modes,int nNumGraphicsModes)
{ //init mode string array
  //allocate memory for graphics mode strings
  g_pszGraphicsModeString=new char*[nNumGraphicsModes];
  for(int i=0; i<nNumGraphicsModes; i++)
    g_pszGraphicsModeString[i]=new char[MODESTRINGLENGTH];
  //put text into graphics mode strings
  char buffer[MODESTRINGLENGTH]; //temporary buffer
  for(i=0; i<nNumGraphicsModes; i++){ //for each mode
    //screen resolution
    sprintf(g_pszGraphicsModeString[i],"%dx%d",modes[i].Width,modes[i].Height);
    //color depth
    int colordepth=0;
    switch(modes[i].Format){
      case D3DFMT_R8G8B8:
        colordepth=24;
        break;
      case D3DFMT_A8R8G8B8:
      case D3DFMT_X8R8G8B8:
        colordepth=32;
        break;
      case D3DFMT_R5G6B5:
      case D3DFMT_X1R5G5B5:
      case D3DFMT_A1R5G5B5:
      case D3DFMT_A4R4G4B4:
        colordepth=16;
    }
    sprintf(buffer,", %d-bit color",colordepth);
    strcat(g_pszGraphicsModeString[i],buffer);
    //refresh rate
    if(modes[i].RefreshRate){ //if not default refresh rate
      sprintf(buffer," (%dHz)",modes[i].RefreshRate);
      strcat(g_pszGraphicsModeString[i],buffer);
    }
  }
} //InitModeStrings

void ReleaseModeStrings(int nNumGraphicsModes){ 
  //reclaim memory from mode string array
  if(g_pszGraphicsModeString!=NULL){
    for(int i=0; i<nNumGraphicsModes; i++)
      delete[]g_pszGraphicsModeString[i];
    delete[]g_pszGraphicsModeString;

  }
} //ReleaseModeStrings

LRESULT CALLBACK ModesDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam){
//message handler for graphics modes dialog box
  WORD w=LOWORD(wParam); //message parameter
  HWND hControl=GetDlgItem(hDlg,IDC_LIST1); //handle to control for list box
  int i; //counter
  int result=LB_ERR; //result
	switch(message){
		case WM_INITDIALOG:
      //put graphics mode strings into list box
      for(i=0; i<g_nNumAcceptableModes; i++)
        SendMessage(hControl,LB_ADDSTRING,0,(LPARAM)(g_pszGraphicsModeString[i]));
		  return TRUE;
    case WM_COMMAND:
      switch(w){
        case IDOK: //ok button
          result=SendMessage(hControl,LB_GETCURSEL,0,0); //return selected index
				  EndDialog(hDlg,result); //close dialog box
          return TRUE;
        case IDCANCEL: //cancel button
				  EndDialog(hDlg,LB_ERR); //close dialog box
          return TRUE;
        case IDC_LIST1: //user clicked on list box
          switch(HIWORD(wParam)){
            case LBN_SELCHANGE: //activate OK button on selection
              EnableWindow(GetDlgItem(hDlg,IDOK),TRUE);
              break;
            case LBN_DBLCLK: //double-click fakes OK button
              SendMessage(hDlg,WM_COMMAND,IDOK,0);
              break;
          }
          return TRUE;
      } //end switch
			break;
	}
  return FALSE;
} //ModesDlgProc

BOOL ChooseGraphicsMode(HINSTANCE hInstance,HWND hwnd,D3DDISPLAYMODE &mode){
   //get information about available graphics modes  
  D3DFORMAT format32,format16; //graphics formats
  int nNumGraphicsModes,nNumGraphicsModes32,nNumGraphicsModes16; //number of modes
  //32-bit graphics modes
  format32=D3DFMT_X8R8G8B8;
  nNumGraphicsModes32=d3dcomobj->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8);
  //16-bit graphics modes 
  nNumGraphicsModes16=d3dcomobj->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_R5G6B5);
  if(nNumGraphicsModes16>0)format16=D3DFMT_R5G6B5;
  else{
    nNumGraphicsModes16=d3dcomobj->GetAdapterModeCount(D3DADAPTER_DEFAULT,D3DFMT_X1R5G5B5);
    if(nNumGraphicsModes16>0)format16=D3DFMT_X1R5G5B5;
  }
  //get list of adapter mode into modes[] array 
  nNumGraphicsModes=nNumGraphicsModes32+nNumGraphicsModes16; //total number of modes
  D3DDISPLAYMODE* modes=new D3DDISPLAYMODE[nNumGraphicsModes]; //create array space
  D3DDISPLAYMODE curmode; //current mode under consideration
  for(int i=0; i<nNumGraphicsModes16; i++){ //for each 16-bit mode
    d3dcomobj->EnumAdapterModes(D3DADAPTER_DEFAULT,format16,i,&curmode); //get specs into curmode
    if(curmode.Width==SCREEN_WIDTH&&curmode.Height==SCREEN_HEIGHT)
      modes[g_nNumAcceptableModes++]=curmode;
  }
  for(i=0; i<nNumGraphicsModes32; i++){ //for each 32-bit mode
    d3dcomobj->EnumAdapterModes(D3DADAPTER_DEFAULT,format32,i,&curmode); //get specs into curmode
    if(curmode.Width==SCREEN_WIDTH&&curmode.Height==SCREEN_HEIGHT)
      modes[g_nNumAcceptableModes++]=curmode;
  }
  //display graphics modes dialog box
  InitModeStrings(modes,nNumGraphicsModes); //create human-readable strings for each mode
  int selectedmode=DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOGMODES),
    hwnd,(DLGPROC)ModesDlgProc); //run dialog box
  ReleaseModeStrings(nNumGraphicsModes); //reclaim mode string memory space
  if(selectedmode==LB_ERR){ //user bailed or no mode selected
    delete[]modes; //reclaim memory for mode list
    return FALSE; //bail and fail
  }
  else{
    D3DDISPLAYMODE result=modes[selectedmode];
    delete[]modes; //reclaim memory for mode list
    mode=result; //return mode
    return TRUE;
  }
} //ChooseGraphicsMode

HWND CreateDefaultWindow(char* name,HINSTANCE hInstance,int nCmdShow){
  //create default full screen black window
  WNDCLASS wc; //window registration info
  //fill in registration information wc
  wc.style=CS_HREDRAW|CS_VREDRAW; //style
  wc.lpfnWndProc=WindowProc; //window message handler
  wc.cbClsExtra=wc.cbWndExtra=0;
  wc.hInstance=hInstance; //instance handle
  wc.hIcon=LoadIcon(hInstance,IDI_APPLICATION); //default icon
  wc.hCursor=NULL; //no cursor
  wc.hbrBackground=NULL; //we will draw background
  wc.lpszMenuName=NULL; //no menu
  wc.lpszClassName=name; //app name provided as parameter
  //register window
  RegisterClass(&wc);
  //create and set up fullscreen window
  HWND hwnd=CreateWindowEx(WS_EX_TOPMOST,name,name,
    WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),
    GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInstance,NULL);
  if(hwnd){ //if successfully created window
    ShowWindow(hwnd,nCmdShow); UpdateWindow(hwnd); //show and update
    SetFocus(hwnd); //get input from keyboard
  }
  return hwnd; //return window handle
} //CreateDefaultWindow

//graphics init functions

HRESULT CreateD3DDevice(HWND hwnd,D3DDISPLAYMODE mode){ //create d3d device
  //set up device specs data structure
  ZeroMemory (&presparams,sizeof(D3DPRESENT_PARAMETERS)); //null it out
  presparams.BackBufferWidth=mode.Width; //width
  presparams.BackBufferHeight=mode.Height; //height
  presparams.BackBufferFormat=mode.Format; //color mode
  presparams.BackBufferCount=1; //one back buffer
  presparams.MultiSampleType=D3DMULTISAMPLE_NONE;
  presparams.SwapEffect=D3DSWAPEFFECT_FLIP; //flip pages
  presparams.hDeviceWindow=NULL; //full screen
  presparams.Windowed=FALSE; //full screen
  presparams.EnableAutoDepthStencil=FALSE; //not needed
  presparams.AutoDepthStencilFormat=(D3DFORMAT)0; //not needed
  presparams.Flags=D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; //can lock buffer
  //refresh rate - use from mode descriptor if available
  if(mode.RefreshRate)
    presparams.FullScreen_RefreshRateInHz=mode.RefreshRate;
  else
    presparams.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
  presparams.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
  //create device
  return d3dcomobj->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,&presparams,&d3ddevice);
}

bool initd3d(HINSTANCE hInstance,HWND hwnd)
{ //init D3D, return TRUE if succeeded
  HRESULT sb; //result from D3D function calls
  //create d3d object
  d3dcomobj=Direct3DCreate9(D3D_SDK_VERSION); //create D3D object
  if(d3dcomobj==NULL)return FALSE; //bail if failed
  //choose graphics format
  D3DDISPLAYMODE displaymode; //display mode
  BOOL bChosenMode=ChooseGraphicsMode(hInstance,hwnd,displaymode);
  if(!bChosenMode)return FALSE; //bail and fail if no mode acceptable
  //create d3d device for mode
  sb=CreateD3DDevice(hwnd,displaymode);
  if(FAILED(sb))
  { //bail if failed
    d3ddevice=NULL; return FALSE;
  }
 // d3ddevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&d3dbackbuffer);		
  //setup backbuffer for menu screens
  //create vertex buffer for background
  sb=d3ddevice->CreateVertexBuffer( 
    4*sizeof(BILLBOARDVERTEX),D3DUSAGE_WRITEONLY,BILLBOARDVERTEX::FVF,
    D3DPOOL_MANAGED,&sbnegx,NULL);
  if(FAILED(sb))return FALSE; //bail if failed
    sb=d3ddevice->CreateVertexBuffer( 
    4*sizeof(BILLBOARDVERTEX),D3DUSAGE_WRITEONLY,BILLBOARDVERTEX::FVF,
    D3DPOOL_MANAGED,&sbposx,NULL);
  if(FAILED(sb))return FALSE; //bail if failed
  sb=d3ddevice->CreateVertexBuffer( 
    4*sizeof(BILLBOARDVERTEX),D3DUSAGE_WRITEONLY,BILLBOARDVERTEX::FVF,
    D3DPOOL_MANAGED,&sbnegz,NULL);
  if(FAILED(sb))return FALSE; //bail if failed
    sb=d3ddevice->CreateVertexBuffer( 
    4*sizeof(BILLBOARDVERTEX),D3DUSAGE_WRITEONLY,BILLBOARDVERTEX::FVF,
    D3DPOOL_MANAGED,&sbposz,NULL);
  if(FAILED(sb))return FALSE; //bail if failed
    sb=d3ddevice->CreateVertexBuffer( 
    4*sizeof(BILLBOARDVERTEX),D3DUSAGE_WRITEONLY,BILLBOARDVERTEX::FVF,
    D3DPOOL_MANAGED,&sbtop,NULL);
  if(FAILED(sb))return FALSE; //bail if failed
  return TRUE; //success exit
} //InitD3D

//graphics restore functions

BOOL RestoreDevice()
{ //restore lost device and all surfaces
  //reset device
  //d3dbackbuffer->Release(); 
  //d3dbackbuffer=NULL;
  if(FAILED(d3ddevice->Reset(&presparams)))
    return FALSE;
  //get new back buffer
  //if(FAILED(d3ddevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&d3dbackbuffer)))
  //   return FALSE;
  setupd3d(); //set D3D default states
  return SUCCEEDED(d3ddevice->TestCooperativeLevel()); //just to be sure
} //RestoreDevice

BOOL DeviceLost()
{ //check for lost device
  return FAILED(d3ddevice->TestCooperativeLevel());
} //DeviceLost