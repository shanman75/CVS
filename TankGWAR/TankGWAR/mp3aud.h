/****************************************************************
mp3aud.h


Header file for an mp3 audio class.

Includes functions to setup an mp3 audio stream.

Last updated: April 8,2004
****************************************************************/

#ifndef mp3aud_h
#define mp3aud_h

#include "StdAfx.h"
#include <dshow.h>

class cmp3stream
{   public:
		cmp3stream(HWND AppWindow);
		int CreateGraph(LPCSTR filename);
		void startsound();
		void stopsound();
		void OnGraphEvent();
		~cmp3stream();
    private:
		IGraphBuilder*	GraphBuilder;
		IMediaControl* MediaControl;
		IMediaEventEx*	MediaEvent;
		IMediaPosition*	MediaPosition;
};
#endif