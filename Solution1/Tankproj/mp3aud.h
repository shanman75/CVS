/****************************************************************
mp3aud.h


Header file for an mp3 audio class.

Includes functions to setup an mp3 audio stream.

Last updated: March 16,2004
****************************************************************/

#include <dshow.h>

class mp3stream
{   public:
		mp3stream(HWND AppWindow);
		int CreateGraph(char *filename);
		void OnGraphEvent();
		~mp3stream();
    private:
		IGraphBuilder*	GraphBuilder;
		IMediaControl*	MediaControl;
		IMediaEventEx*	MediaEvent;
		IMediaPosition*	MediaPosition;
};
