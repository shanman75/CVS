/*******************************************************************

wavsound.cpp

implementation file for a .wav class that uses directsound

created April 24,2004
modified April 24,2004

*******************************************************************/

#include "stdafx.h"
#include "wavsound.h"

cwavsound::cwavsound(HWND hwnd)
{	if(SUCCEEDED(DirectSoundCreate8(NULL,&ds8,NULL)))
    {  if(SUCCEEDED(ds8->SetCooperativeLevel(hwnd,DSSCL_PRIORITY)))
       {  loadbuffers();
	   }
    }
}

void cwavsound::play(gamesound gs)
{	soundbuffer[gs]->Play(0,0,0);
}

void cwavsound::stopsounds()
{	for(int i=0;i<maxsounds;i++)
	  soundbuffer[i]->Stop();
}

cwavsound::~cwavsound()
{	for(int i=0;i<maxsounds;i++)
    {  soundbuffer[i]->Release();
       soundbuffer[i]=NULL;
	}
	ds8->Release();
}

void cwavsound::loadbuffers()
{   WAVEFORMATEX wavfile;
    DSBUFFERDESC bufdes;

	//wave format descriptor
    memset(&wavfile,0,sizeof(WAVEFORMATEX));
	wavfile.nAvgBytesPerSec=22050;  //blockalign * samplespersec
	wavfile.nBlockAlign=2;			//channels*bps/8
	wavfile.nChannels=1;
	wavfile.nSamplesPerSec=11025;
	wavfile.wBitsPerSample=16;
	wavfile.wFormatTag=WAVE_FORMAT_PCM;

    //direct sound buffer descriptor
    memset(&bufdes,0,sizeof(DSBUFFERDESC));
	bufdes.dwSize=sizeof(DSBUFFERDESC);
	bufdes.dwFlags=DSBCAPS_STATIC;
	bufdes.dwBufferBytes=DSBSIZE_MIN;
	bufdes.dwReserved=0;
//	bufdes.guid3DAlgorithm=NULL;
	bufdes.lpwfxFormat=&wavfile;
	for(int i=0;i<maxsounds;i++)
	{	soundbuffer[i]=NULL;
		ds8->CreateSoundBuffer(&bufdes,&(soundbuffer[i]),NULL);
	}

 /* HMMIO hmfr;
  MMCKINFO parent,child;
  BYTE *sound=NULL; //temporary buffer to hold sound data
  //reclaim memory from buffer, if already used
  //init parent and child MMCKINFOs
  parent.ckid=(FOURCC)0;
  parent.cksize=0;
  parent.fccType=(FOURCC)0;
  parent.dwDataOffset=0;
  parent.dwFlags=0;
  child=parent;
  //open sound file
  hmfr=mmioOpen("resources\sounds\turret.wav",NULL,MMIO_READ|MMIO_ALLOCBUF);

  //descend into the RIFF
  parent.fccType=mmioFOURCC('W','A','V','E');
  if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF)){
    mmioClose(hmfr,0);
  }
  //descend to the WAVEfmt
  child.ckid=mmioFOURCC('f','m','t',' ');
  if(mmioDescend(hmfr,&child,&parent,0)){
    mmioClose(hmfr,0);
  }
  //read the WAVEFMT from the wave file
  if(mmioRead(hmfr,(char*)&wavfile,sizeof(wavfile))!=sizeof(wavfile)){
    mmioClose(hmfr,0);
  }
  //ascend back to RIFF level
  if(mmioAscend(hmfr,&child,0)){
    mmioClose(hmfr,0);
  }
  //descend to the data chunk
  child.ckid=mmioFOURCC('d','a','t','a');
  if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK)){
    mmioClose(hmfr,0);
  }
  //read the wave data
  mmioRead(hmfr,(char *)sound,child.cksize);
  //close file and return
  mmioClose(hmfr,0);

  
	LPVOID w1,w2; //write pointer (use 2 for buffer wraparound)
    DWORD l1,l2; //length of sound to be written to write pointers

	soundbuffer[tankmove]->Lock(0,DSBSIZE_MIN,&w1,&l1,&w2,&l2,0);
	     CopyMemory(w1,"resources\sound\turret.wav",l1);
	soundbuffer[tankmove]->Unlock(w1,l1,w2,l2);

/*	soundbuffer[bullet]->Lock(0,DSBSIZE_MAX,&w1,&l1,&w2,&l2,0);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[bullet]->Unlock(w1,l1,w2,l2);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[abomb]->Lock(0,DSBSIZE_MAX,&w1,&l1,&w2,&l2,0);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[abomb]->Unlock(w1,l1,w2,l2);

	soundbuffer[scud]->Lock(0,DSBSIZE_MAX,&w1,&l1,&w2,&l2,0);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[scud]->Unlock(w1,l1,w2,l2);

	soundbuffer[amram]->Lock(0,DSBSIZE_MAX,&w1,&l1,&w2,&l2,0);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[amram]->Unlock(w1,l1,w2,l2);

	soundbuffer[fbomb]->Lock(0,DSBSIZE_MAX,&w1,&l1,&w2,&l2,0);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[fbomb]->Unlock(w1,l1,w2,l2);

	soundbuffer[explosion]->Lock(0,DSBSIZE_MAX,&w1,&l1,&w2,&l2,0);
      CopyMemory(w1,"turret.wav",l1);
	soundbuffer[explosion]->Unlock(w1,l1,w2,l2);
*/
}