#pragma once

#include "stdafx.h"
#include "3DObject.h"
#include "3DObjectTank.h"
#include "ObjMgr.h"
#include "D3DInput.h"

struct st_PlayerState {
   char *name;
   c3DObject *object;
   float money;
};


class cGameState
{
public:
  cGameState(void);
  ~cGameState(void);
  void GetInput();
  void AddPlayer(BOOL human=false);
private:     
     st_PlayerState m_PlayerState[MAX_PLAYERS];
     int m_numplayers;

};

extern cGameState *g_GameState;
