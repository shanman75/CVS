/****************************************************************
mp3aud.h


Header file for an mp3 audio class.

Includes functions to setup an mp3 audio stream.

Last updated: April 8,2004
****************************************************************/

#ifndef mp3_h
#define mp3_h

#include <dshow.h>

class mp3stream
{   public:
		mp3stream(HWND AppWindow);
		int CreateGraph(LPCSTR filename);
		void startsound();
		void stopsound();
		void OnGraphEvent();
		~mp3stream();
    private:
		IGraphBuilder*	GraphBuilder;
		IMediaControl* MediaControl;
		IMediaEventEx*	MediaEvent;
		IMediaPosition*	MediaPosition;
};

#endif