#pragma once

#include "stdafx.h"
#include "3DObject.h"
#include "3DObjectTank.h"
#include "3DObjectMissile.h"
#include "3DObjectExplosion.h"
#include "ObjMgr.h"
#include "D3DInput.h"
#include "Camera.h"
#include "CameraBehindTank.h"
#include "CameraBehindMissile.h"
#include "CameraAboveExplosion.h"
#include "CameraAboveHit.h"
#include "Skybox.h"
#include "Terrain.h"

struct st_PlayerState {
   char *name;
   c3DObject *object;
   c3DObject *msl_object;
   c3DObjectExplosion *exp_object;
   float money;
   float health;
};


class cGameState
{
public:
  enum STATES { TARGETING, FIRING, EXPLODING};
  enum MAINSTATES { MENU, PRELEVEL, LEVEL, POSTLEVEL, ENDGAME };
  cGameState(void);
  ~cGameState(void);
  void move(void);
  void paintbg(void);
  void paint(void);

  void GetInput();
  void AddPlayer(BOOL human=false);
  void GetCurrentExpState(D3DXVECTOR3 *pos, D3DXVECTOR3 *scale);
  void GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient);
  void GetCurrentMissileState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient, D3DXVECTOR3 *velocity);
  float GetTerrainHeight(float x, float z) { if (m_terrain) return m_terrain->GetHeight(x,z); 
  else return 1.0f;}

  cCamera *GetCurrentCamera(void);
  void SetCurrentCamera(cCamera *cam) { if (cam !=NULL) m_curcamera =cam; }

private:     
     cSkyBox *m_skybox;
     cTerrain *m_terrain;
     //cTerrain *m_big_terrain;
     st_PlayerState m_PlayerState[MAX_PLAYERS];
     int m_numplayers;
     int m_currentplayer;
     cCamera *m_curcamera;
     cCameraBehindTank     m_camBehindTank;
     cCameraBehindMissile  m_camBehindMissile;
     cCameraAboveExplosion m_camAboveExplosion;
     enum STATES m_gstate;
     int RoundNumber;

};

extern cGameState *g_GameState;
