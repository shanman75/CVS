//sound.cpp: sound manager

//Copyright Ian Parberry, 2002
//Last updated September 27, 2002

#include <stdio.h>
#include "sound.h"

CSoundManager::CSoundManager(HWND hwnd){ //constructor
  m_nCount=0; //no sounds yet
  //null out sound buffers and flags
  for(int i=0; i<MAX_SOUNDS; i++){
    m_lpBuffer[i]=NULL; m_nCopyCount[i]=0;
  }
  //start direct sound
  m_bOperational=SUCCEEDED(
    DirectSoundCreate8(NULL,&m_lpDirectSound,NULL));
  if(!m_bOperational)return;
  m_bOperational=SUCCEEDED(
    m_lpDirectSound->SetCooperativeLevel(hwnd,DSSCL_PRIORITY));
}

CSoundManager::~CSoundManager(){ //destructor
  if(!m_bOperational)return;
  clear(); //clear all buffers
  (void)m_lpDirectSound->Release(); //release direct sound
}


void CSoundManager::beginframe(){ //to be called at start of frame
  for(int i=0; i<m_nCount; i++)m_bStartedThisFrame[i]=FALSE;
}

void CSoundManager::clear(){ //clear all sounds
  if(!m_bOperational)return;
  stop(); //stop all sounds (paranoia)
  for(int i=0; i<m_nCount; i++){ //for each sound
    for(int j=0; j<m_nCopyCount[i]; j++){ //for each copy
      m_lpBuffer[i][j]->Release(); //release the sound
      m_lpBuffer[i][j]=NULL; //probably not needed
    }
    //reclaim memory
    delete[]m_lpBuffer[i];
  }
  m_nCount=0; //no sounds left (hopefully)
}

void CSoundManager::load(char *filename,int copies){
//load sound from file
//copies is how many copies of sound are to play simultaneously
  int length; //length of sound
  BYTE *sound=NULL; //temporary buffer to hold sound data
  //bail out if necessary
  if(!m_bOperational)return; //bail if not initialized
  if(m_nCount>=MAX_SOUNDS)return; //bail if no space left
  //load sound data from file
  length=LoadSound(filename,&sound); //load sound from file
  CreateBuffer(m_nCount,length,copies); //create buffers
  LoadBuffer(m_nCount,sound,length); //load into buffer
  CopyBuffer(m_nCount,copies); //make copies of contents
  //clean up and exit
  delete[]sound; //delete temporary sound buffer
  m_bStartedThisFrame[m_nCount]=FALSE; //not started yet
  m_nCount++; //increment counter
}

void CSoundManager::play(int index,BOOL looping){ //play sound
  if(!m_bOperational)return; //bail if not initialized
  if(index<0||index>=m_nCount)return; //bail if bad index
  if(m_bStartedThisFrame[index])return; //bail if already playing
  int copy=0; //current copy
  DWORD status; //status of that copy
  //get status of first copy
  if(FAILED(m_lpBuffer[index][copy]->GetStatus(&status)))
    status=DSBSTATUS_PLAYING; //assume playing if failed
  //find next unplayed copy, if any
  while(copy<m_nCopyCount[index]&&
  (status&DSBSTATUS_PLAYING)){ //while current copy in use
    if(++copy<m_nCopyCount[index]) //go to next copy
      if(FAILED(m_lpBuffer[index][copy]->GetStatus(&status)))
        status=DSBSTATUS_PLAYING; //assume playing if failed
  }
  //play copy
  if(copy<m_nCopyCount[index]){ //if unused copy found
    m_lpBuffer[index][copy]->
      Play(0,0,looping?DSBPLAY_LOOPING:0); //play it
    m_bStartedThisFrame[index]=TRUE; //record fact that we started it
  }
}

void CSoundManager::stop(int index){ //stop playing sound
  if(!m_bOperational)return; //bail if not initialized 
  if(index<0||index>=m_nCount)return; //bail if bad index
  for(int i=0; i<m_nCopyCount[index]; i++){ //for each copy
    m_lpBuffer[index][i]->Stop(); //stop playing
    m_lpBuffer[index][i]->SetCurrentPosition(0); //rewind
  }
}

void CSoundManager::stop(void){ //stop playing sound
  if(!m_bOperational)return; //bail if not initialized 
  for(int index=0; index<m_nCount; index++) //for each sound
    stop(index); //stop that sound
}

BOOL CSoundManager::CopyBuffer(int index,int copies){ 
//make copies of buffer
  BOOL result=TRUE; //TRUE if everything went OK
  if(!m_bOperational)return FALSE; //bail if not initialized
  m_nCopyCount[index]=copies; //record number of copies
  for(int i=1; i<copies; i++) //for each copy
    result=result&& //copy the sound
      SUCCEEDED(m_lpDirectSound->
        DuplicateSoundBuffer(*m_lpBuffer[index],
        &(m_lpBuffer[index][i])));
  return result;
}

BOOL CSoundManager::CreateBuffer(int index,int length,int copies){
  if(!m_bOperational)return FALSE; //bail if not initialized
  DSBUFFERDESC dsbdesc;
  PCMWAVEFORMAT pcmwf;
  if(length<=0)return FALSE; //bail if length info wrong
  //init pcmwf, wave format descriptor
  memset(&pcmwf,0,sizeof(PCMWAVEFORMAT));
  pcmwf.wf.wFormatTag=WAVE_FORMAT_PCM;
  pcmwf.wf.nChannels=DS_CHANSPERSAMPLE;
  pcmwf.wf.nSamplesPerSec=DS_SAMPLERATE;
  pcmwf.wf.nBlockAlign=
    DS_CHANSPERSAMPLE*DS_BITSPERSAMPLE/8;
  pcmwf.wf.nAvgBytesPerSec=
    pcmwf.wf.nSamplesPerSec*pcmwf.wf.nBlockAlign;
  pcmwf.wBitsPerSample=DS_BITSPERSAMPLE;
  //init dsbdesc, direct sound buffer descriptor
  memset(&dsbdesc,0,sizeof(DSBUFFERDESC));
  dsbdesc.dwSize=sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags=DSBCAPS_STATIC;
  dsbdesc.dwBufferBytes=length;
  dsbdesc.lpwfxFormat=(LPWAVEFORMATEX)&pcmwf;
  //create sound buffers and return success code
  m_lpBuffer[index]=new LPDIRECTSOUNDBUFFER[copies];
  for(int i=0; i<copies; i++)m_lpBuffer[index][i]=NULL;
  return SUCCEEDED(m_lpDirectSound->
    CreateSoundBuffer(&dsbdesc,m_lpBuffer[index],NULL));
  return TRUE; //debug
}

BOOL CSoundManager::LoadBuffer(int index,BYTE *sound,int length){
  if(!m_bOperational)return FALSE; //bail if not initialized
  LPVOID w1,w2; //write pointer (use 2 for buffer wraparound)
  DWORD l1,l2; //length of sound to be written to write pointers
  LPDIRECTSOUNDBUFFER buffer=*m_lpBuffer[index];
  if(length<=0)return FALSE; //bail if length info wrong
  if(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0) //lock down buffer
  ==DSERR_BUFFERLOST){ //if buffer lost
    buffer->Restore(); //restore, then try again
    if(FAILED(buffer->Lock(0,length,&w1,&l1,&w2,&l2,0)))
      return FALSE; //abort if failed the second time
  }
  CopyMemory(w1,sound,l1); //load first half 
  if(w2!=NULL)CopyMemory(w2,sound+l1,l2); //load  second half
  //unlock the buffer and return
  if(FAILED(buffer->Unlock(w1,l1,w2,l2)))return FALSE;
  return TRUE;
}

int CSoundManager::LoadSound(char *filename,BYTE **buffer){
  int size=0;
  if(!m_bOperational)return 0; //bail if not initialized
  HMMIO hmfr;
  MMCKINFO parent,child;
  WAVEFORMATEX wfmtx;
  //reclaim memory from buffer, if already used
  delete[]*buffer;
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
}