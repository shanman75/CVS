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

void cwavsound::play(gamesound gs, BOOL looping)
{	
  if(gs<0||gs>=maxsounds)return; //bail if bad index
  int copy=0; //current copy
  DWORD status; //status of that copy

  //get status of first copy
  if(FAILED(soundbuffer[gs][copy]->GetStatus(&status)))
    status=DSBSTATUS_PLAYING; //assume playing if failed
  //find next unplayed copy, if any
  while(copy<m_nCopyCount[gs]&&
  (status&DSBSTATUS_PLAYING)){ //while current copy in use
    if(++copy<m_nCopyCount[gs]) //go to next copy
      if(FAILED(soundbuffer[gs][copy]->GetStatus(&status)))
        status=DSBSTATUS_PLAYING; //assume playing if failed
  }

  //play copy
  if(copy<m_nCopyCount[gs]){ //if unused copy found
    soundbuffer[gs][copy]->
      Play(0,0,looping?DSBPLAY_LOOPING:0); //play it
    m_bStartedThisFrame[gs]=TRUE; //record fact that we started it
  }

//  soundbuffer[gs]->Play(0,0,0);
}

void cwavsound::stopsounds()
{	
  for(int index=0;index<maxsounds;index++)
	  for(int j=0; j<m_nCopyCount[index]; j++){ //for each copy
      soundbuffer[index][j]->Stop(); //stop playing
      soundbuffer[index][j]->SetCurrentPosition(0); //rewind
  }
}

cwavsound::~cwavsound()
{	for(int i=0;i<maxsounds;i++)
    {  //soundbuffer[i]->Release();
       soundbuffer[i]=NULL;
	}
	ds8->Release();
}

int cwavsound::loadsound(char *filename,BYTE **buffer)
{// FILE * logging;
 // logging=fopen("test2.txt","w");

  int size=0;
  HMMIO hmfr;
  MMCKINFO parent,child;
  WAVEFORMATEX wfmtx;
  //reclaim memory from buffer, if already used
  //delete [] buffer;
  //init parent and child MMCKINFOs
  parent.ckid=(FOURCC)0;
  parent.cksize=0;
  parent.fccType=(FOURCC)0;
  parent.dwDataOffset=0;
  parent.dwFlags=0;
  child=parent;
  //open sound file

  hmfr=mmioOpen(filename,NULL,MMIO_READ|MMIO_ALLOCBUF);

  if(hmfr==NULL)return NULL;
  //descend into the RIFF
  parent.fccType=mmioFOURCC('W','A','V','E');
  if(mmioDescend(hmfr,&parent,NULL,MMIO_FINDRIFF)){
    mmioClose(hmfr,0); return NULL; //not a wave file
  }
  //descend to the WAVEfmt
  child.ckid=mmioFOURCC('f','m','t',' ');
  if(mmioDescend(hmfr,&child,&parent,0)){
    mmioClose(hmfr,0); return NULL; //file has no fmt chunk
  }
  //read the WAVEFMT from the wave file
  if(mmioRead(hmfr,(char*)&wfmtx,sizeof(wfmtx))!=sizeof(wfmtx)){
    mmioClose(hmfr,0); return NULL; //unable to read fmt chunk
  }
  //check wave format
  if(wfmtx.wFormatTag!=WAVE_FORMAT_PCM){
    mmioClose(hmfr,0); return NULL; //WAVE file is not PCM format
  }
  //ascend back to RIFF level
  if(mmioAscend(hmfr,&child,0)){
    mmioClose(hmfr,0); return NULL; //unable to ascend
  }
  //descend to the data chunk
  child.ckid=mmioFOURCC('d','a','t','a');
  if(mmioDescend(hmfr,&child,&parent,MMIO_FINDCHUNK)){
    mmioClose(hmfr,0); return NULL; //WAVE file has no data chunk
  }
  //grab memory to store sound
  size=child.cksize;
  *buffer=new BYTE[size];
  if(*buffer==NULL){
    mmioClose(hmfr,0); return NULL; //out of memory
  }
  //read the wave data
  if((int)mmioRead(hmfr,(char *)*buffer,size)!=size){
    //data read failed
    mmioClose(hmfr,0); delete[]*buffer; return NULL; 
  }
  //close file and return
  mmioClose(hmfr,0);
  return size;
 // fclose(logging);
}


void cwavsound::loadbuffers()
{   WAVEFORMATEX wavfile;
    DSBUFFERDESC bufdes;
	BYTE *sound=NULL; //temporary buffer to hold sound data
	LPVOID w1,w2; //write pointer (use 2 for buffer wraparound)
	DWORD l1,l2; //length of sound to be written to write pointers
  LPDIRECTSOUNDBUFFER tmpbuff;
//	FILE * logging;
//	logging=fopen("test.txt","w");

	//wave format descriptor
  memset(&wavfile,0,sizeof(WAVEFORMATEX));
	wavfile.wFormatTag=WAVE_FORMAT_PCM;
	wavfile.nChannels=mono;
	wavfile.nSamplesPerSec=frequency;
	wavfile.wBitsPerSample=soundbitquality;
	wavfile.nBlockAlign=wavfile.nChannels*wavfile.wBitsPerSample/8;		
	wavfile.nAvgBytesPerSec=wavfile.nBlockAlign*wavfile.nSamplesPerSec;
	
  //direct sound buffer descriptor
  memset(&bufdes,0,sizeof(DSBUFFERDESC));
	bufdes.dwSize=sizeof(DSBUFFERDESC);
	bufdes.dwFlags=DSBCAPS_STATIC;
	bufdes.dwReserved=0;
	bufdes.lpwfxFormat=(LPWAVEFORMATEX)&wavfile;
	
	int length=loadsound("resource\\sounds\\turret.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
  makecopies(tmpbuff,tankmove,1);
  tmpbuff->Release();

	length=loadsound("resource\\sounds\\shell.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,shell,4);
  tmpbuff->Release();

	length=loadsound("resource\\sounds\\abomb.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,abomb,4);
  tmpbuff->Release();

	length=loadsound("resource\\sounds\\scud.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,scud,4);
  tmpbuff->Release();

	length=loadsound("resource\\sounds\\amram.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,amram,4);
  tmpbuff->Release();

  length=loadsound("resource\\sounds\\fbomb.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,fbomb,4);
  tmpbuff->Release();

  length=loadsound("resource\\sounds\\explosion.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,explosion,4);
  tmpbuff->Release();

  // Changes to buffer format required for menu sounds...
  wavfile.nSamplesPerSec=22050;
	wavfile.wBitsPerSample=8;
	wavfile.nBlockAlign=1*8/8;		
	wavfile.nAvgBytesPerSec=wavfile.nBlockAlign*wavfile.nSamplesPerSec;

  length=loadsound("resource\\sounds\\menumove.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,mnu_move,4);
  tmpbuff->Release();

	length=loadsound("resource\\sounds\\menuselect.wav",&sound);
	bufdes.dwBufferBytes=length;
	tmpbuff=NULL;
	if(SUCCEEDED(ds8->CreateSoundBuffer(&bufdes,&tmpbuff,NULL)))
	{	tmpbuff->Lock(0,length,&w1,&l1,&w2,&l2,0);
			CopyMemory(w1,sound,l1);
		if(w2!=NULL)
			CopyMemory(w2,(sound+l1),l2); //load  second half
		tmpbuff->Unlock(w1,l1,w2,l2);
	}
	makecopies(tmpbuff,mnu_select,4);
  tmpbuff->Release();

  
//	fclose(logging);
}

BOOL cwavsound::makecopies(LPDIRECTSOUNDBUFFER buff, int index,int copies){ 
//make copies of buffer
  BOOL result=TRUE; //TRUE if everything went OK

  soundbuffer[index]=new LPDIRECTSOUNDBUFFER[copies];
  for(int i=0; i<copies; i++)soundbuffer[index][i]=NULL;

  m_nCopyCount[index]=copies; //record number of copies
  for(int i=0; i<copies; i++) //for each copy
    result=result&& //copy the sound
      SUCCEEDED(ds8->
        DuplicateSoundBuffer(buff,
        &(soundbuffer[index][i])));
  return result;
}
