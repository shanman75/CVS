/****************************************************************
mp3aud.cpp


Implementation file for an mp3 audio class.

Includes functions to setup an mp3 audio stream.

Last updated: April 8,2004
****************************************************************/

#include "StdAfx.h"
#include "mp3aud.h"

cmp3stream::cmp3stream(HWND AppWindow)
//initialize an mp3 file to play
{  	GraphBuilder=NULL;
	MediaControl=NULL;
	MediaEvent=NULL;
	MediaPosition=NULL;
	HRESULT hr;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                          IID_IGraphBuilder,(void**)&GraphBuilder);
	GraphBuilder->QueryInterface(IID_IMediaControl, (void**)&MediaControl);
	GraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&MediaEvent);
 	GraphBuilder->QueryInterface(IID_IMediaPosition, (void**)&MediaPosition);
	MediaEvent->SetNotifyWindow((OAHWND)AppWindow, WM_USER, 0);
	MediaEvent->SetNotifyFlags(0);	// turn on notifications
}

int cmp3stream::CreateGraph(LPCSTR filename)
{   WCHAR *file;
    file=new WCHAR [64];
	HRESULT hr;
    MultiByteToWideChar(CP_ACP,0,filename,-1,file,64);
    hr = GraphBuilder->RenderFile(file,0);
	delete [] file;
	if (FAILED(hr))
		return -1;
	else
	{   MediaControl->Run();
		return 0;
	}
}

void cmp3stream::startsound()
{	MediaControl->Run();
}

void cmp3stream::stopsound()
{	MediaControl->Stop();
}

void cmp3stream::OnGraphEvent()
{
	long EventCode, Param1, Param2;
	while (MediaEvent->GetEvent(&EventCode, &Param1, &Param2, 0) != E_ABORT)
	{
		switch (EventCode)
		{
        case EC_COMPLETE:            
            // here when media is completely done playing
            MediaPosition->put_CurrentPosition(0);   // reset to beginning
            break;
		default:
			break;
		}	
		MediaEvent->FreeEventParams(EventCode, Param1, Param2);
	}
}

cmp3stream::~cmp3stream()
{  if(MediaPosition) MediaPosition->Release();
   if(MediaEvent) MediaEvent->Release();
   if(MediaControl) MediaControl->Release();
   if(GraphBuilder) GraphBuilder->Release();
}