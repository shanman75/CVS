#pragma once

#include "stdafx.h"
#include "3DObject.h"
#include "3DObjectTank.h"
#include "ObjMgr.h"
#include "D3DInput.h"
#include "Camera.h"

struct st_PlayerState {
   char *name;
   c3DObject *object;
   float money;
};


class cGameState
{
public:
  enum STATES { MENU,PRELEVEL,TURN,FIRING,POSTLEVEL };
  cGameState(void);
  ~cGameState(void);
  void GetInput();
  void AddPlayer(BOOL human=false);
  void GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient);

  cCamera *GetCurrentCamera(void) { return m_curcamera; }
  void SetCurrentCamera(cCamera *cam) { if (cam !=NULL) m_curcamera =cam; }

private:     
     st_PlayerState m_PlayerState[MAX_PLAYERS];
     int m_numplayers;
     int m_currentplayer;
     cCamera *m_curcamera;
     enum STATES m_gstate;

};

extern cGameState *g_GameState;
