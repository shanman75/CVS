//sound.h: header class for sound.cpp
//CSoundManager allows you to play multiple copies of each
//sound simultaneously

//Copyright Ian Parberry, 2002
//Last updated September 27, 2002

#ifndef __SOUND__
#define __SOUND__

#include <dsound.h> //direct sound
#include "sndlist.h" //list of sound names

#define MAX_SOUNDS 64 //maximum number of different sounds

#define DS_NUMCHANNELS 8 //number of channels
#define DS_CHANSPERSAMPLE 1 //mono sound
#define DS_BITSPERSAMPLE 8 //8 bit sound
#define DS_SAMPLERATE 22050 //22KHz sampling

class CSoundManager{ //sound manager
  private:
    int m_nCount; //number of sounds loaded
    LPDIRECTSOUND8 m_lpDirectSound; //directSound object
    LPLPDIRECTSOUNDBUFFER m_lpBuffer[MAX_SOUNDS]; //sound buffers
    int m_nCopyCount[MAX_SOUNDS]; //num copies of each sound
    BOOL m_bStartedThisFrame[MAX_SOUNDS]; //TRUE if started in this frame
    BOOL m_bOperational; //directSound initialized correctly
    BOOL CreateBuffer(int index,int length,int copies);
    BOOL LoadBuffer(int index,BYTE *buffer,int length);
    int LoadSound(char *filename,BYTE **sound); //load from file
    BOOL CopyBuffer(int index,int copies); //copy sound
  public:
    CSoundManager(HWND hwnd); //constructor
    ~CSoundManager(); //destructor
    void beginframe(void); //to be called at start of frame
    void clear(); //clear all sounds
    void load(char *filename,int copies=1); //load from file
    void play(int index,BOOL looping=FALSE); //play sound
    void stop(int index); //stop playing sound
    void stop(void); //stop playing all sounds
};

#endif
