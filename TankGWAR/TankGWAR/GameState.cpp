#include "StdAfx.h"
#include "gamestate.h"
#include <stdio.h>

cGameState *g_GameState;

void cGameState::move(void)
{
  if (m_gstate == FIRING)
  {
         D3DXVECTOR3 msl_pos = m_PlayerState[m_currentplayer].msl_object->m_position;
         if (m_terrain->GetHeight(msl_pos.x,msl_pos.z) > msl_pos.y) {
           g_ObjMgr->del(m_PlayerState[m_currentplayer].msl_object);
           m_PlayerState[m_currentplayer].msl_object = NULL;
           m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
           SetCurrentCamera(&m_camBehindTank);
           m_gstate = TURN;
         }
  }
}

void cGameState::paintbg(void)
{
  m_skybox->Paint();
  m_terrain->Paint();
}

void cGameState::paint(void)
{
  static char szPower[255];
  static char szHeight[255];


  sprintf(szPower,"Pos (%.1f,%.1f,%.1f) Power %.2f\n",
       ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position.x,
       ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position.y,
       ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position.z,
       ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_firePower
     );
  g_D3DObject->DrawTextStr(50,500,D3DCOLOR_XRGB(240,240,50),szPower);
  sprintf(szHeight,"Turret %.1f x %.1f",
    ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate,
    ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight
    );
  g_D3DObject->DrawTextStr(50,470,D3DCOLOR_XRGB(240,240,50),szHeight);
}
void cGameState::GetInput(void)
{
  c3DObjectTank *tmpP = NULL;

  g_D3DInput->GetInput((cTerrain *)m_terrain);

  switch (m_gstate) {
     case TURN:
        tmpP = (c3DObjectTank *)m_PlayerState[m_currentplayer].object;
        if (g_D3DInput->KeyDown(DIK_UP))
          tmpP->event(c3DObjectTank::UP);
        else if (g_D3DInput->KeyDown(DIK_DOWN))
          tmpP->event(c3DObjectTank::DOWN);
        if (g_D3DInput->KeyDown(DIK_LEFT))
          tmpP->event(c3DObjectTank::LEFT);
        else if (g_D3DInput->KeyDown(DIK_RIGHT))
          tmpP->event(c3DObjectTank::RIGHT);
        if (g_D3DInput->KeyDown(DIK_MINUS))
          tmpP->event(c3DObjectTank::PWRDN);
        else if (g_D3DInput->KeyDown(DIK_EQUALS))
          tmpP->event(c3DObjectTank::PWRUP);
        if (g_D3DInput->MouseDown(0) | g_D3DInput->KeyDown(DIK_F) | g_D3DInput->KeyDown(DIK_SPACE)) {
          m_PlayerState[m_currentplayer].msl_object = tmpP->Fire(c3DObjectTank::MISSILE);          
          SetCurrentCamera(&m_camBehindMissile);
          m_gstate = FIRING;
        }
        break;
     case FIRING:
       if (g_D3DInput->KeyDown(DIK_N))
       {
         m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
         SetCurrentCamera(&m_camBehindTank);
         m_gstate = TURN;
       }
       break;
     default:
       break;
  }
}

void cGameState::GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient)
{
  *pos =      ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position;
  orient->x = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate;
  orient->y = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight;
  orient->z = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

void cGameState::GetCurrentMissileState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient, D3DXVECTOR3 *velocity)
{
  *pos   =    ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_position;
  *orient =    ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_orient;
  *velocity =    ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_velocity;
  //orient->y = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_barrelHeight;
  //orient->z = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

cGameState::cGameState(void)
{
  m_numplayers=0;
  m_terrain = new cTerrain();
  m_skybox = new cSkyBox();
  m_terrain->RandomizeMesh();
  SetCurrentCamera(new cCameraBehindTank);
}

cGameState::~cGameState(void)
{
  SAFE_DELETE(m_terrain);
  SAFE_DELETE(m_skybox);
}

void cGameState::AddPlayer(BOOL human)
{
  c3DObject *tmpP = NULL;
  m_currentplayer = 0;

  m_gstate = cGameState::TURN;

  float t_x, t_y, t_z;

  for (int x =0 ; x < 3; x++)  {
    tmpP = new c3DObjectTank();
    t_x = rand()%1200-600;
    t_z = rand()%1200-600;
    t_y = m_terrain->GetHeight(t_x,t_z);
    tmpP->pos(D3DXVECTOR3((float)(rand()%200-100),(float)t_y,(float)(rand()%200-100)));
    m_PlayerState[m_numplayers].object = tmpP;
    g_ObjMgr->add(m_PlayerState[m_numplayers].object);  
    m_numplayers++;
  }
}

cCamera * cGameState::GetCurrentCamera() { 
  return m_curcamera; 
}
