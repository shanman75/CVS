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
#include "CameraAboveTerrain.h"
#include "CameraAboveHit.h"
#include "Skybox.h"
#include "Terrain.h"
#include "Texture.h"

enum LIVINGSTATE { ALIVE, DEAD, DYING };

struct st_PlayerState {
   int numweapons[c3DObjectMissile::MSLNUM];
   char *name;
   c3DObjectTank *object;
   c3DObjectMissile *msl_object;
   c3DObjectExplosion *exp_object;
   c3DObjectMissile::MSLTYPE msl_cur_type;
   float money;
   float health;
   enum LIVINGSTATE livingstate;
   bool camabove;
   float camabovezoom;
};


class cGameState
{
public:
  enum STATES { NOTHING, TARGETING, FIRING, EXPLODING};
  enum MAINSTATES { MENU, PRELEVEL, LEVEL, POSTLEVEL, ENDGAME };
  cGameState(void);
  ~cGameState(void);
  void move(void);
  void paintbg(void);
  void paint(void);
  void NextPlayer(void);

  void GetInput();
  void AddPlayer(BOOL human=false);
  void GetCurrentExpState(D3DXVECTOR3 *pos, D3DXVECTOR3 *scale, float *radius);
  void GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient);
  void GetCurrentMissileState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient, D3DXVECTOR3 *velocity);
  void GetCurrentCamAboveZoom(float *);
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
     void NextWeapon(int t_dir = 1);

     st_PlayerState m_PlayerState[MAX_PLAYERS];
     int m_numplayers;
     int m_currentplayer;
     cCamera *m_curcamera;
     cCameraBehindTank     m_camBehindTank;
     cCameraBehindMissile  m_camBehindMissile;
     cCameraAboveExplosion m_camAboveExplosion;
     cCameraAboveTerrain m_camAboveTerrain;
     cCamera             m_camera;
     enum STATES m_gstate;
     enum MAINSTATES m_mainstate;
     int RoundNumber;

     void _Init(void);
     void _Delete(void);


     void AssignHits();
     void DropTanks();
     void KillDeadTanks();

     //static LPDIRECT3DTEXTURE9	  m_statusbartex;
     static CTexture *m_statusbartex;
     static CTexture *m_preroundBk;
     static CTexture *m_preroundMousePtr;
     static CTexture *m_preroundRegButton[2];
     
     c3DObjectMissile *m_tmissile;
};

extern cGameState *g_GameState;
