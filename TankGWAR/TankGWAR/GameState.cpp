#include "StdAfx.h"
#include "gamestate.h"


cGameState *g_GameState;

void cGameState::GetInput(void)
{
  c3DObjectTank *tmpP = NULL;

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
          tmpP->Fire(c3DObjectTank::MISSILE);
          m_gstate = FIRING;
        }
        break;
     case FIRING:
       if (g_D3DInput->KeyDown(DIK_N))
       {
         m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
         m_gstate = TURN;
       }
       break;
     default:
       break;
  }
}

void cGameState::GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient)
{
  *pos =    ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position;
  orient->x = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate;
  orient->y = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight;
  orient->z = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

cGameState::cGameState(void)
{
  m_numplayers=0;
}

cGameState::~cGameState(void)
{
}

void cGameState::AddPlayer(BOOL human)
{
  c3DObject *tmpP = NULL;
  tmpP = new c3DObjectTank();
  tmpP->pos(D3DXVECTOR3(0,7,4));
  m_PlayerState[m_numplayers].object = tmpP;
  m_currentplayer = 0;
  g_ObjMgr->add(m_PlayerState[m_numplayers].object);
  m_numplayers++;

  m_gstate = cGameState::TURN;

  for (int x =0 ; x < 2; x++)  {
    tmpP = new c3DObjectTank();
    tmpP->pos(D3DXVECTOR3(rand()%200-100,7,rand()%200-100));
    m_PlayerState[m_numplayers].object = tmpP;
    g_ObjMgr->add(m_PlayerState[m_numplayers].object);  
    m_numplayers++;
  }
}