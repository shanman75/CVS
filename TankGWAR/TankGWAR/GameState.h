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

struct st_LevelState {
  enum COMPUTER_AI { SEEKER, MEAN, NOVICE, EXPERT, RANDOM, AI_MAX };
   int numHumans;   static const int maxHumans = 4;
   int numComputers;   static const int maxComputers = 5;
   int numHumansTMP;   int numComputersTMP;   

   COMPUTER_AI aiComputers[maxComputers];
   int curComputer;
   long numHills;    static const long maxHills = 4;
   long numDirt;    static const long maxDirt = 4;
   int startingMoney;  static const int maxMoney = 19;
   int windLevel;   static const int maxWind = 4;
   int numLevels;   static const int maxLevels = 100;
                    static const int minLevels = 1;
};

struct st_PlayerState {
   int numweapons[c3DObjectMissile::MSLNUM];
   enum st_LevelState::COMPUTER_AI ai_type;
   char *name;
   c3DObjectTank *object;
   c3DObjectMissile *msl_object;
   c3DObjectExplosion *exp_object;
   c3DObjectMissile::MSLTYPE msl_cur_type;
   float money;
   float health;
   bool iscomputer;

   enum LIVINGSTATE livingstate;
   bool camabove;
   float camabovezoom;
};


char * strComputer_AI(st_LevelState::COMPUTER_AI ai_in);
char * strLevel_WIND(int wind);
char * strLevel_MONEY(int money);
char * strLevel_HILLS(int hills);
char * strLevel_DIRT(int dirt);

class cGameState
{
public:
  enum STATES { NOTHING, TARGETING, FIRING, EXPLODING};
  enum MAINSTATES { MAINMENU, CREDITS, HELP, GAMESETUP, PRELEVEL, LEVEL, POSTLEVEL, ENDGAME };
  enum MAINMENUBUTT { MM_NEWGAME, MM_CREDITS, MM_QUIT, MM_HELP, MM_NONE };
  enum GAMESETUPBUTT { GS_HUMANPL_UP, GS_HUMANPL_DN, GS_CMPPL_UP, GS_CMPPL_DN,
                       GS_CMPAI_UP, GS_CMPAI_DN, GS_HILLS_UP, GS_HILLS_DN,
                       GS_DIRT_UP, GS_DIRT_DN, GS_WIND_UP, GS_WIND_DN,
                       GS_MONEY_UP, GS_MONEY_DN, GS_NLVL_UP, GS_NLVL_DN,
                       GS_STARTGAME, GS_CANCEL, GS_NONE };
  cGameState(void);
  ~cGameState(void);
  void move(void);
  void paintbg(void);
  void paint(void);
  void NextPlayer(void);

  void GetInput();
  void AddPlayer(BOOL human=false);
  void AddPlayer_old(BOOL human=false);
  void GetCurrentExpState(D3DXVECTOR3 *pos, D3DXVECTOR3 *scale, float *radius);
  void GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient);
  void GetCurrentMissileState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient, D3DXVECTOR3 *velocity);
  void GetCurrentCamAboveZoom(float *);
  inline float GetTerrainHeight(float x, float z) { 
      if (m_terrain) return m_terrain->GetHeight(x,z); 
      else return 1.0f;
  }

  cCamera *GetCurrentCamera(void);
  void SetCurrentCamera(cCamera *cam) { if (cam !=NULL) m_curcamera =cam; }

  void OnLostDevice(void);
  void OnResetDevice(void);
private:     
     cSkyBox *m_skybox;
     cTerrain *m_terrain;
     //cTerrain *m_big_terrain;
     void NextWeapon(int t_dir = 1);
     void _InitLevelState(void);
     void _InitGame(void);

     st_PlayerState m_PlayerState[MAX_PLAYERS];
     st_LevelState m_LevelState;
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
     static CTexture *m_preroundLogo;
     static CTexture *m_preroundRegButton[2];

     static CTexture *m_creditsBk;
     static CTexture *m_helpBk;

     static CTexture *m_spinner[3];
     static CTexture *m_textfield[1];
     CTimer m_tmSpinner;

     enum MAINMENUBUTT m_mainmenubutt;
     enum GAMESETUPBUTT m_gamesetupbutt;
     
     c3DObjectMissile *m_tmissile;
};

extern cGameState *g_GameState;