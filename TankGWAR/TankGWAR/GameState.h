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
#include "TExture.h"

struct st_PlayerState {
   char *name;
   c3DObjectTank *object;
   c3DObjectMissile *msl_object;
   c3DObjectExplosion *exp_object;
   c3DObjectMissile::MSLTYPE msl_cur_type;
   float money;
   float health;
   enum LIVINGSTATE livingstate;
};


class cGameState
{
public:
  enum STATES { TARGETING, FIRING, EXPLODING};
  enum MAINSTATES { MENU, PRELEVEL, LEVEL, POSTLEVEL, ENDGAME };
  enum LIVINGSTATE { ALIVE, DEAD, DYING };
  cGameState(void);
  ~cGameState(void);
  void move(void);
  void paintbg(void);
  void paint(void);

  void GetInput();
  void AddPlayer(BOOL human=false);
  void GetCurrentExpState(D3DXVECTOR3 *pos, D3DXVECTOR3 *scale, float *radius);
  void GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient);
  void GetCurrentMissileState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient, D3DXVECTOR3 *velocity);
  float GetTerrainHeight(float x, float z) { if (m_terrain) return m_terrain->GetHeight(x,z); 
  else return 1.0f;}

  cCamera *GetCurrentCamera(void);
  void SetCurrentCamera(cCamera *cam) { if (cam !=NULL) m_curcamera =cam; }

  void OnLostDevice(void);
  void OnResetDevice(void);
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

     void _Init(void);
     void _Delete(void);


     void AssignHits();
     void DropTanks();
     void KillDeadTanks();

     //static LPDIRECT3DTEXTURE9	  m_statusbartex;
     static CTexture *m_statusbartex;
};

extern cGameState *g_GameState;
