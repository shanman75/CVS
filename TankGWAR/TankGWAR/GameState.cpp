#include "StdAfx.h"
#include "gamestate.h"


cGameState *g_GameState;

void cGameState::GetInput(void)
{
  c3DObjectTank *tmpP = NULL;

  tmpP = (c3DObjectTank *)m_PlayerState[0].object;
  if (g_D3DInput->KeyDown(DIK_SPACE))
    tmpP->Fire(c3DObjectTank::MISSILE);
  if (g_D3DInput->KeyDown(DIK_UP))
    tmpP->event(c3DObjectTank::UP);
  if (g_D3DInput->KeyDown(DIK_DOWN))
    tmpP->event(c3DObjectTank::DOWN);
  if (g_D3DInput->KeyDown(DIK_LEFT))
    tmpP->event(c3DObjectTank::LEFT);
  if (g_D3DInput->KeyDown(DIK_RIGHT))
    tmpP->event(c3DObjectTank::RIGHT);
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
  tmpP->pos(D3DXVECTOR3(0,0,4));
  m_PlayerState[m_numplayers].object = tmpP;
  g_ObjMgr->add(m_PlayerState[m_numplayers].object);
  m_numplayers++;
}