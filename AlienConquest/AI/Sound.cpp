#include "StdAfx.h"
#include "sound.h"

LPDIRECTSOUND8 CSound::m_dsound = NULL;
int CSound::m_dsound_init = 0;

CSound::CSound(char *filename)
{
	if (!m_dsound_init++) {
		m_dsound_init++;
		OutputDebugString("Creating DirectSound device\n");
		// Init d3d sound device
		if (FAILED(DirectSoundCreate8(NULL,&m_dsound,NULL))) {
			OutputDebugString("DirectSound creation failure\n"); 
			return;
		}
		if (FAILED(m_dsound->SetCooperativeLevel(hwnd,DSSCL_PRIORITY))){
			OutputDebugString("DirectSound Set Coop level failed\n");
			return;
		}
	}

	m_looping = 1;
	load(filename);
}

CSound::~CSound(void)
{
	if (!--m_dsound_init) {
		// Cleanup...you are the last
		if (m_dsound)
		m_dsound->Release();
		m_dsound=NULL;
	}
}

void CSound::load(char *filename)
{
  int length; //length of sound
  BYTE *sound=NULL; //temporary buffer to hold sound data
  //bail out if necessary
  length=LoadSound(filename,&sound); //load sound from file
  CreateBuffer(1,length,1); //create buffers
 // LoadBuffer(1,sound,length); //load into buffer
  //CopyBuffer(1,copies); //make copies of contents
  //clean up and exit
  delete[]sound; //delete temporary sound buffer
//  m_nCount++; //increment counter
}

void CSound::play()
{
	if (!m_sound) return;
	DWORD status;

    if(FAILED(m_sound->GetStatus(&status)))
		status=DSBSTATUS_PLAYING;
	if (!(status & DSBSTATUS_PLAYING))
    m_sound->
      Play(0,0,m_looping?DSBPLAY_LOOPING:0); //play it
}

void CSound::stop()
{
	m_sound->Stop();
}

int CSound::LoadSound(char *filename,BYTE **buffer){

  // Taken directly from Dr Parberry code...why re-invent?

  int size=0;
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

BOOL CSound::CreateBuffer(int index,int length,int copies){
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
  m_dsound->CreateSoundBuffer(&dsbdesc,&m_sound,NULL);
  return TRUE; //debug
}

BOOL CSound::LoadBuffer(int index,BYTE *sound,int length){
  LPVOID w1,w2; //write pointer (use 2 for buffer wraparound)
  DWORD l1,l2; //length of sound to be written to write pointers
  LPDIRECTSOUNDBUFFER buffer=m_sound;
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
