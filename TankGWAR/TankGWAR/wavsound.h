/*******************************************************************

wavsound.h

header file for a .wav class that uses directsound

created April 24,2004
modified April 24,2004

*******************************************************************/

#ifndef wavsound_h
#define wavsound_h

#include <dsound.h>
#include <Mmreg.h>
#include <stdlib.h>
#include <stdio.h>

#define mono 1
#define stereo 2
#define frequency 11025
#define soundbitquality 16

enum gamesound{tankmove,shell,abomb,scud,amram,fbomb,explosion,mnu_move,mnu_select,maxsounds};

class cwavsound
{ //class to play a wav
	public:
	  cwavsound(HWND hwnd);
	  void play(gamesound, BOOL looping = FALSE);
	  void stopsounds();
    void stop(int index);
	  ~cwavsound();

    int m_nCopyCount[maxsounds]; //num copies of each sound
    BOOL m_bStartedThisFrame[maxsounds]; //TRUE if started in this frame
    BOOL makecopies(LPDIRECTSOUNDBUFFER buff, int index,int copies); //copy sound

  private:
	  void loadbuffers();
	  int loadsound(char *filename,BYTE **buffer); 
	  LPDIRECTSOUND8 ds8;
	  LPLPDIRECTSOUNDBUFFER soundbuffer[maxsounds];
};
#endif
