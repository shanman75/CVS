#pragma once

//#define DIRECTSOUND_VERSION 0x800
#include <windows.h>
#include <mmsystem.h>
//#include <mmreg.h>
#include <dsound.h>

#define DS_NUMCHANNELS 8 //number of channels
#define DS_CHANSPERSAMPLE 1 //mono sound
#define DS_BITSPERSAMPLE 8 //8 bit sound
#define DS_SAMPLERATE 22050 //22KHz sampling

class CSound
{
public:
	CSound(char *filename);
	~CSound(void);
	void play(void);
	void stop(void);
	void load(char *);
private:
	static LPDIRECTSOUND8 m_dsound;
	static int m_dsound_init;
	BOOL m_looping;

	LPDIRECTSOUNDBUFFER m_sound;

	int LoadSound(char *filename,BYTE **buffer);
	BOOL CreateBuffer(int index,int length,int copies=1);
	BOOL LoadBuffer(int index,BYTE *sound,int length);
};
