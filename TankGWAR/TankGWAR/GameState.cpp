#include "StdAfx.h"
#include "gamestate.h"
#include "MainWnd.h"
#include "wavsound.h"
#include <stdio.h>

extern cwavsound *wav;

cGameState *g_GameState;
//LPDIRECT3DTEXTURE9 cGameState::m_statusbartex = NULL;
CTexture *cGameState::m_statusbartex = NULL;
CTexture *cGameState::m_preroundBk = NULL;
CTexture *cGameState::m_preroundMousePtr =NULL;
CTexture *cGameState::m_preroundRegButton[2];
CTexture *cGameState::m_preroundSmButton[2];
CTexture *cGameState::m_preroundLogo = NULL;
CTexture *cGameState::m_helpBk = NULL;

CTexture *cGameState::m_creditsBk = NULL;

// Window controls
CTexture *cGameState::m_spinner[3];
CTexture *cGameState::m_textfield[1];

extern bool sh_FPS;
extern bool sh_MAP;

void cGameState::move(void)
{
  static CTimer tmrState;
  D3DXVECTOR3 msl_pos;
//  c3DObject *tmpP;
//  m_gstate = TARGETING;

  if (m_mainstate==MAINSTATES::LEVEL)
  switch (m_gstate) 
  {
  case TARGETING: break;
  case FIRING:
         msl_pos = m_PlayerState[m_currentplayer].msl_object->m_position;
         if (m_terrain->GetHeight(msl_pos.x,msl_pos.z) > msl_pos.y ||
             fabs(msl_pos.x) > 170.0f ||
             fabs(msl_pos.z) > 170.0f) {
           // Create New Explosion
           m_PlayerState[m_currentplayer].exp_object = 
             new c3DObjectExplosion(
                    c3DObjectMissile::GetMissileExpRadius(m_PlayerState[m_currentplayer].msl_cur_type),
                    4300.0f,
                    c3DObjectMissile::GetMissileExpColor(m_PlayerState[m_currentplayer].msl_cur_type)
                    );
           m_PlayerState[m_currentplayer].exp_object->pos(msl_pos);
           if ( fabs(msl_pos.x) <= 170.0f && fabs(msl_pos.z) <= 170.0f)
              m_PlayerState[m_currentplayer].exp_object->m_position.y = m_terrain->GetHeight(
                        m_PlayerState[m_currentplayer].exp_object->m_position.x,
                        m_PlayerState[m_currentplayer].exp_object->m_position.z );
           g_ObjMgr->del(m_PlayerState[m_currentplayer].msl_object);
           g_ObjMgr->add(m_PlayerState[m_currentplayer].exp_object);
           m_PlayerState[m_currentplayer].msl_object = NULL;
           SetCurrentCamera(&m_camAboveExplosion);
           m_camAboveExplosion.m_camtime.Reset();
           m_gstate = EXPLODING;
           tmrState.setInterval(9000);
           tmrState.Reset();
         }
    break;
  case EXPLODING:
    if (tmrState.CmpTime()) {
          m_terrain->FlattenSphere(m_PlayerState[m_currentplayer].exp_object->m_position,
            m_PlayerState[m_currentplayer].exp_object->m_radius);
          AssignHits();
          DropTanks();
          KillDeadTanks();
          
          g_ObjMgr->del(m_PlayerState[m_currentplayer].exp_object);
          m_gstate = KILLDEADTANKS;
          tmrState.setInterval(100);
          tmrState.Reset();
          m_savecurrentplayer = m_currentplayer;
    }
    break;
  case KILLDEADTANKS:
    // Show Dead Tanks
    if (tmrState.CmpTime()) {
      int dyingtank;
      dyingtank = GetFirstDyingTank();
      if (dyingtank == -1) {
        m_currentplayer = m_savecurrentplayer;
        NextPlayer();
        SetPlayerCamera();
        m_gstate = LEVELCOMPLETE;
      }
      else {
        // Found a dying tank!!!
        m_PlayerState[dyingtank].object->FadeOut(3000.0f);
        //wav->play(playerdead);
        m_PlayerState[dyingtank].livingstate = DEAD;
        m_PlayerState[dyingtank].health = 0;
        tmrState.setInterval(3100.0f);
        m_currentplayer = dyingtank;
        m_PlayerState[dyingtank].camabovezoom = 0.1f;
        SetCurrentCamera(&m_camAboveTerrain);
      }
    }
    break;
  case LEVELCOMPLETE:
       int livetanks;
       livetanks = 0;
       for (int x = 0; x < m_numplayers; x++)
         if (m_PlayerState[x].livingstate == ALIVE) livetanks++;
       if (livetanks <= 1) { m_mainstate = POSTLEVEL; UpdateScores(); }
       else { m_gstate = TARGETING; }
    break;
  default:
    break;
  }
}

int cGameState::GetFirstDyingTank()
{
  int x = 0; 
  while (x < m_numplayers)
  {
    if (m_PlayerState[x].livingstate == DYING) return x;
    x++;
  }
  return -1;
}

void cGameState::SetPlayerCamera(void)
{
  if (m_PlayerState[m_currentplayer].camabove)
    SetCurrentCamera(&m_camAboveTerrain);
  else
    SetCurrentCamera(&m_camBehindTank);
}
void cGameState::NextPlayer(void)
{
  int tplay = m_numplayers;
  m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
  while(m_PlayerState[m_currentplayer].livingstate != ALIVE && tplay-- > 0)
    m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
}

BOOL PointInRect(D3DXVECTOR2 *pnt, RECT *rct)
{
  if ( (pnt->x >= rct->left) && (pnt->x <= rct->right))
    if ((pnt->y >= rct->top) && (pnt->y <= rct->bottom))
      return true;
  return false;
}

int PointInSpinner(D3DXVECTOR2 *pnt, RECT *rct)
{
  RECT trect;
  char debg[500];

  SetRect(rct,rct->left+4,rct->top+3,rct->left+29,rct->top+31);  
  if (PointInRect(pnt,rct)) {
    SetRect(&trect,rct->left+4,rct->top+3,rct->left+29,rct->top+16);
//    sprintf(debg,"UP - Checking (%.1f,%.1f) against (%i,%i,%i,%i)\n",pnt->x,pnt->y,
//                                trect.top,trect.left,trect.bottom,trect.right); OutputDebugString(debg);
    if (PointInRect(pnt,&trect)) return 1;   // Up
    SetRect(&trect,rct->left+4,rct->top+19,rct->left+29,rct->top+31);
//    sprintf(debg,"DN - Checking (%.1f,%.1f) against (%i,%i,%i,%i)\n",pnt->x,pnt->y,
//                                trect.top,trect.left,trect.bottom,trect.right); OutputDebugString(debg);
    if (PointInRect(pnt,&trect)) return 2;   // Down
    return 0;
  }
  else return 0;
}

void cGameState::paintbg(void)
{
  D3DXVECTOR2 mscreen;
  RECT mrect;

  g_D3DInput->MouseScreen(&mscreen);

  if (m_mainstate == MAINSTATES::MAINMENU) {
    m_mainmenubutt = MAINMENUBUTT::MM_NONE;
    int but_ng;
    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_preroundBk->Paint(0.0f,0.0f);
    m_preroundLogo->Paint(200.0f,0.0f);

    const int st_y = 196;
    // New Game Button
    SetRect(&mrect,200,st_y+(41+30)*0,600,st_y+(41+30)*0+41);
    but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
    if (but_ng == 0) m_mainmenubutt = MAINMENUBUTT::MM_NEWGAME;
    m_preroundRegButton[but_ng]->Paint(200.0f,(float)(st_y+(41+30)*0));
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"New Game",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    // Credits button
    SetRect(&mrect,200,st_y+(41+30)*1,600,st_y+(41+30)*1+41);
    but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
    if (but_ng == 0) m_mainmenubutt = MAINMENUBUTT::MM_CREDITS;
    m_preroundRegButton[but_ng]->Paint(200.0f,(float)(st_y+(41+30)*1));
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Credits",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    // Help button
    SetRect(&mrect,200,st_y+(41+30)*2,600,st_y+(41+30)*2+41);
    but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
    if (but_ng == 0) m_mainmenubutt = MAINMENUBUTT::MM_HELP;
    m_preroundRegButton[but_ng]->Paint(200.0f,(float)(st_y+(41.0f+30.0f)*2));
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Help",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    // Quit button
    SetRect(&mrect,200,st_y+(41+30)*3,600,st_y+(41+30)*3+41);
    but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
    if (but_ng == 0) m_mainmenubutt = MAINMENUBUTT::MM_QUIT;
    m_preroundRegButton[but_ng]->Paint(200.0f,(float)(st_y+(41.0f+30.0f)*3));
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Quit",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    m_preroundMousePtr->Paint(&mscreen);
//    m_preroundButtons->Paint(&mouserect,&mscreen);
    g_D3DObject->m_pd3dxSprite->End();
  } 
  else if (m_mainstate == MAINSTATES::ENDGAME)
  {
    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_preroundBk->Paint(0.0f,0.0f);

    SetRect(&mrect,200,200,600,400);
    g_D3DObject->pFont->DrawText(g_D3DObject->m_pd3dxSprite,randomwintxt(),-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
    SetRect(&mrect,200,400,600,600);
    g_D3DObject->pFont->DrawText(g_D3DObject->m_pd3dxSprite,getwinnername(),-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
    
    m_preroundMousePtr->Paint(&mscreen);
    g_D3DObject->m_pd3dxSprite->End();
  }
  else if (m_mainstate == MAINSTATES::CREDITS)
  {
    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_creditsBk->Paint(0.0f,0.0f);
    g_D3DObject->m_pd3dxSprite->End();
  }
  else if (m_mainstate == MAINSTATES::HELP)
  {
    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_helpBk->Paint(0.0f,0.0f);
    g_D3DObject->m_pd3dxSprite->End();
  }
  else if (m_mainstate == MAINSTATES::GAMESETUP)
  {
    int st_y = 40;
    D3DXVECTOR2 tp_scr;
    int but_ng;
    static char t_strbuff[255];

    m_gamesetupbutt = GAMESETUPBUTT::GS_NONE;
    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_preroundBk->Paint(0.0f,0.0f);
    
    // Heading
    SetRect(&mrect,0,0,800,100);
    g_D3DObject->pFont->DrawText(g_D3DObject->m_pd3dxSprite,"Game Setup",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));


    // Human Spinner
    tp_scr = D3DXVECTOR2(70.0f,150.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Humans",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_HUMANPL_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_HUMANPL_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    sprintf(t_strbuff,"%i",m_LevelState.numHumans);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));


    // Number of Levels Spinner
    tp_scr = D3DXVECTOR2(270.0f,150.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Levels",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_NLVL_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_NLVL_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    sprintf(t_strbuff,"%i",m_LevelState.numLevels);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));

    // Hills Spinner
    tp_scr = D3DXVECTOR2(70.0f,270.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Hilliness",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_HILLS_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_HILLS_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    sprintf(t_strbuff,"%i",m_LevelState.numHills);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,strLevel_HILLS(m_LevelState.numHills),-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));


    // Crap Spinner
    tp_scr = D3DXVECTOR2(270.0f,270.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Jaggedness",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_DIRT_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_DIRT_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,strLevel_DIRT(m_LevelState.numDirt),-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));


    // Wind Level
    tp_scr = D3DXVECTOR2(70.0f,400.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Wind Level",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_WIND_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_WIND_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,strLevel_WIND(m_LevelState.windLevel),-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));

    // Starting Money
    tp_scr = D3DXVECTOR2(270.0f,400.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Money",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_MONEY_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_MONEY_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,strLevel_MONEY(m_LevelState.startingMoney),-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));


    // Computers Spinner
    tp_scr = D3DXVECTOR2(480.0f,150.0f);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Computers",-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1) m_gamesetupbutt = GAMESETUPBUTT::GS_CMPPL_UP;           // Make next state HM_UP
    else if (but_ng == 2) m_gamesetupbutt = GAMESETUPBUTT::GS_CMPPL_DN;      // Make next state HM_DN
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    sprintf(t_strbuff,"%i",m_LevelState.numComputers);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));


    // Computer Types Spinner
    for (int c = 0; c < m_LevelState.numComputers; c++) {
    tp_scr = D3DXVECTOR2(620.0f,150.0f+70.0f*c);
    // Heading
    SetRect(&mrect,tp_scr.x,tp_scr.y-40,tp_scr.x+150,tp_scr.y+35-40);
    sprintf (t_strbuff,"Computer %i",c+1);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
      &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    SetRect(&mrect,tp_scr.x+110,tp_scr.y,0,0); 
    but_ng = PointInSpinner(&mscreen,&mrect);
    if (but_ng == 1)    
    { m_gamesetupbutt = GAMESETUPBUTT::GS_CMPAI_UP; m_LevelState.curComputer = c; }
    else if (but_ng == 2) 
    { m_gamesetupbutt = GAMESETUPBUTT::GS_CMPAI_DN; m_LevelState.curComputer = c; }
    m_spinner[but_ng]->Paint(tp_scr.x+110,tp_scr.y);

    m_textfield[0]->Paint(tp_scr.x,tp_scr.y);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+100,tp_scr.y+35);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,strComputer_AI(m_LevelState.aiComputers[c]),-1,
      &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,(float)128/255,0.0f,1.0f));
    }
 
    // Buttons
    // Cancel Button
    tp_scr = D3DXVECTOR2(402.0f,500.0f);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+400,tp_scr.y+45);
    but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
    if (but_ng == 0) m_gamesetupbutt = GAMESETUPBUTT::GS_CANCEL;
    m_preroundRegButton[but_ng]->Paint(&tp_scr);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Cancel",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    // Start Button
    tp_scr = D3DXVECTOR2(2.0f,500.0f);
    SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+400,tp_scr.y+45);
    but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
    if (but_ng == 0) m_gamesetupbutt = GAMESETUPBUTT::GS_STARTGAME;
    m_preroundRegButton[but_ng]->Paint(&tp_scr);
    g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Start",-1,
      &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));


    m_preroundMousePtr->Paint(&mscreen);
    g_D3DObject->m_pd3dxSprite->End();
  }
  else if (m_mainstate == MAINSTATES::POSTLEVEL)
  {
    static char t_strbuff[255];
    int st_y = 50;
    int spc_y = 20;
    int ht_y = 31;

    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_preroundBk->Paint(0.0f,0.0f);

    sprintf(t_strbuff,"Stats for Level %i of %i",RoundNumber,m_LevelState.numLevels);
    SetRect(&mrect,2,5,800,45);
      g_D3DObject->pFont->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));

    SetRect(&mrect,2,550,800,45);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Click to continue",-1,
        &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));

    for (int c = 0; c < m_numplayers; c++) {
      // Names
      if (c == 0) {
        SetRect(&mrect,2,st_y+(ht_y+spc_y)*c,200,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Name",-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,2,st_y+(ht_y+spc_y)*(c+1),200,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,m_PlayerState[c].name,-1,
        &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

      // Kills
      if (c == 0) {
        SetRect(&mrect,0,st_y+(ht_y+spc_y)*c,180,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Kills",-1,
          &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,0,st_y+(ht_y+spc_y)*(c+1),180,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      sprintf(t_strbuff,"%i",m_LevelState.numKills[c]);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

      // Damage
      if (c == 0) {
        SetRect(&mrect,0,st_y+(ht_y+spc_y)*c,300,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Damage",-1,
          &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,0,st_y+(ht_y+spc_y)*(c+1),300,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      sprintf(t_strbuff,"%.1f",m_LevelState.numDamage[c]);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

      // Cash Won
      if (c == 0) {
        SetRect(&mrect,0,st_y+(ht_y+spc_y)*c,400,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Cash",-1,
          &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,0,st_y+(ht_y+spc_y)*(c+1),400,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      sprintf(t_strbuff,"$%.2f M",m_LevelState.numMoney[c]/1000000);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

      // Score
      if (c == 0) {
        SetRect(&mrect,0,st_y+(ht_y+spc_y)*c,500,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Score",-1,
          &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,0,st_y+(ht_y+spc_y)*(c+1),500,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      sprintf(t_strbuff,"%.1f",m_LevelState.numScore[c]);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

// Totals
      // Total Cash
      if (c == 0) {
        SetRect(&mrect,0,st_y+(ht_y+spc_y)*c,650,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Total Cash",-1,
          &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,0,st_y+(ht_y+spc_y)*(c+1),650,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      sprintf(t_strbuff,"$%.2fM",m_PlayerState[c].money/1000000);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

      // Total Score
      if (c == 0) {
        SetRect(&mrect,0,st_y+(ht_y+spc_y)*c,785,st_y+(ht_y+spc_y)*c+ht_y);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Total Score",-1,
          &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.4f,0.0f,1.0f,1.0f));
      }
      SetRect(&mrect,0,st_y+(ht_y+spc_y)*(c+1),785,st_y+(ht_y+spc_y)*(c+1)+ht_y);
      sprintf(t_strbuff,"%.1f",m_PlayerState[c].score);
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_RIGHT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

    }
    m_preroundMousePtr->Paint(&mscreen);
    g_D3DObject->m_pd3dxSprite->End();
  }
  else if (m_mainstate == MAINSTATES::PRELEVEL) {
    static char t_strbuff[255];
    int st_y = 50;
    int spc_y = 20;
    int spc_x = 20;
    int ht_y = 31;
    D3DXVECTOR2 tp_scr;
    int but_ng;

    m_prelevelbutt = PRELEVELBUTT::PL_NONE;
    
    g_D3DObject->m_pd3dxSprite->Begin(0);
    m_preroundBk->Paint(0.0f,0.0f);

    sprintf(t_strbuff,"Purchase Order for Level %i of %i",RoundNumber,m_LevelState.numLevels);
    SetRect(&mrect,2,5,800,45);
      g_D3DObject->pFont->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
        &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));

    // Start Level Button
      st_y = 543;
      SetRect(&mrect,200,st_y+(41+30)*0,600,st_y+(41+30)*0+41);
      but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
      if (but_ng == 0) m_prelevelbutt = PRELEVELBUTT::PL_STARTLEVEL;
      m_preroundRegButton[but_ng]->Paint(200.0f,(float)(st_y+(41+30)*0));
      g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Start Level",-1,
        &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));


    for (int u=0; u < m_LevelState.numHumans; u++)
    {
        int humanid = 0;
        spc_y = 180;
        ht_y = 100;

        humanid = GetHumanNumber(u);
        tp_scr = D3DXVECTOR2(40.0f+spc_y*u,50.0f);
        SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+400,tp_scr.y+45);
        sprintf(t_strbuff,"%s",m_PlayerState[humanid].name);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
        sprintf(t_strbuff,"$%.1fM",m_PlayerState[humanid].money/1000000);
        SetRect(&mrect,tp_scr.x,tp_scr.y+20,tp_scr.x+400,tp_scr.y+45+20);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
        
        // Scud Button
        tp_scr = D3DXVECTOR2(40.0f+spc_y*u,50.0f+ht_y*1);
        SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+129,tp_scr.y+45);
        but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
        if (but_ng == 0) 
        {m_prelevelbutt = PRELEVELBUTT::PL_WEAPONBUY;
         m_prelevelplayer = humanid;
         m_prelevelmsl = c3DObjectMissile::MSLTYPE::SCUD;
        }
        m_preroundSmButton[but_ng]->Paint(&tp_scr);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"Scud",-1,
          &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Own: %i",m_PlayerState[humanid].numweapons[c3DObjectMissile::MSLTYPE::SCUD]);
        SetRect(&mrect,tp_scr.x,tp_scr.y-38,tp_scr.x+129,tp_scr.y+45-38);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Cost: $%.1fM",c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::SCUD)/1000000);
        SetRect(&mrect,tp_scr.x,tp_scr.y+33,tp_scr.x+129,tp_scr.y+45+33);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));


        // AMRAM Button
        tp_scr = D3DXVECTOR2(40.0f+spc_y*u,50.0f+ht_y*2);
        SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+129,tp_scr.y+45);
        but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
        if (but_ng == 0) 
        {m_prelevelbutt = PRELEVELBUTT::PL_WEAPONBUY;
         m_prelevelplayer = humanid;
         m_prelevelmsl = c3DObjectMissile::MSLTYPE::AMRAM;
        }
        m_preroundSmButton[but_ng]->Paint(&tp_scr);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"AM-RAM",-1,
          &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Own: %i",m_PlayerState[humanid].numweapons[c3DObjectMissile::MSLTYPE::AMRAM]);
        SetRect(&mrect,tp_scr.x,tp_scr.y-38,tp_scr.x+129,tp_scr.y+45-38);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Cost: $%.1fM",c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::AMRAM)/1000000);
        SetRect(&mrect,tp_scr.x,tp_scr.y+33,tp_scr.x+129,tp_scr.y+45+33);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

        // Funkie Button
        tp_scr = D3DXVECTOR2(40.0f+spc_y*u,50.0f+ht_y*3);
        SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+129,tp_scr.y+45);
        but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
        if (but_ng == 0) 
        {m_prelevelbutt = PRELEVELBUTT::PL_WEAPONBUY;
         m_prelevelplayer = humanid;
         m_prelevelmsl = c3DObjectMissile::MSLTYPE::FUNKIEBOMB;
        }
        m_preroundSmButton[but_ng]->Paint(&tp_scr);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"F-Bomb",-1,
          &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Own: %i",m_PlayerState[humanid].numweapons[c3DObjectMissile::MSLTYPE::FUNKIEBOMB]);
        SetRect(&mrect,tp_scr.x,tp_scr.y-38,tp_scr.x+129,tp_scr.y+45-38);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Cost: $%.1fM",c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::FUNKIEBOMB)/1000000);
        SetRect(&mrect,tp_scr.x,tp_scr.y+33,tp_scr.x+129,tp_scr.y+45+33);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));

        // Atom Bomb Button
        tp_scr = D3DXVECTOR2(40.0f+spc_y*u,50.0f+ht_y*4);
        SetRect(&mrect,tp_scr.x,tp_scr.y,tp_scr.x+129,tp_scr.y+45);
        but_ng = PointInRect(&mscreen,&mrect) ? 0 : 1;
        if (but_ng == 0) 
        {m_prelevelbutt = PRELEVELBUTT::PL_WEAPONBUY;
         m_prelevelplayer = humanid;
         m_prelevelmsl = c3DObjectMissile::MSLTYPE::ATOMBOMB;
        }
        m_preroundSmButton[but_ng]->Paint(&tp_scr);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,"A-Bomb",-1,
          &mrect,DT_CENTER|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Own: %i",m_PlayerState[humanid].numweapons[c3DObjectMissile::MSLTYPE::ATOMBOMB]);
        SetRect(&mrect,tp_scr.x,tp_scr.y-38,tp_scr.x+129,tp_scr.y+45-38);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER,D3DXCOLOR(0.0f,0.0f,1.0f,1.0f));
        sprintf(t_strbuff,"Cost: $%.1fM",c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::ATOMBOMB)/1000000);
        SetRect(&mrect,tp_scr.x,tp_scr.y+33,tp_scr.x+129,tp_scr.y+45+33);
        g_D3DObject->pFont_StatusBar->DrawText(g_D3DObject->m_pd3dxSprite,t_strbuff,-1,
          &mrect,DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_NOCLIP,D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));


    }
    m_preroundMousePtr->Paint(&mscreen);
    g_D3DObject->m_pd3dxSprite->End();
  }
  else if (m_mainstate == MAINSTATES::LEVEL) {
    m_skybox->Paint();
    m_terrain->Paint();
  }
}

int cGameState::GetHumanNumber(int human)
{
  int cnt = 0;
  int ret = 0;

  for (int x = 0; x < m_numplayers; x++)
    if (!m_PlayerState[x].iscomputer)
      if(cnt++ == human) return x;

  return 0;
}
void cGameState::paint(void)
{
  static char szPower[255];
  static char szHeight[255];


  if(0){
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

  if (m_mainstate == MAINSTATES::LEVEL) {
  g_D3DObject->m_pd3dxSprite->Begin(0);
  m_statusbartex->Paint(0.0f,0.0f);
  g_D3DObject->m_pd3dxSprite->End();

  static RECT healthrect;
  SetRect(&healthrect,28,9,66,28);
  static char healthstr[255];
  sprintf(healthstr,"%.0f%",m_PlayerState[m_currentplayer].health);
  g_D3DObject->DrawTextStr_StatusBar(&healthrect,D3DCOLOR_XRGB(255,255,255),healthstr, DT_RIGHT | DT_NOCLIP | DT_VCENTER);

  static RECT moneyrect;
  SetRect(&moneyrect,106,9,215,28);
  static char moneystr[255];
  sprintf(moneystr,"%.2f M",m_PlayerState[m_currentplayer].money/1000000);
  g_D3DObject->DrawTextStr_StatusBar(&moneyrect,D3DCOLOR_XRGB(255,255,255),moneystr, DT_RIGHT | DT_NOCLIP | DT_VCENTER);

  static RECT playerrect;
  SetRect(&playerrect,254,9,347,28);
  //sprintf(moneystr,"%.2f M",m_PlayerState[m_currentplayer].money/1000000);
  g_D3DObject->DrawTextStr_StatusBar(&playerrect,D3DCOLOR_XRGB(255,255,255),
                     m_PlayerState[m_currentplayer].name, DT_CENTER | DT_NOCLIP | DT_VCENTER);
 
//((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate,
//    ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight
  static char rotatestr[10];
  static RECT rotaterect;
  SetRect(&rotaterect,383,9,432,28);
  sprintf(rotatestr,"%i",(int)(D3DXToDegree(((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate)));
  g_D3DObject->DrawTextStr_StatusBar(&rotaterect,D3DCOLOR_XRGB(255,255,255),
                     rotatestr, DT_RIGHT | DT_NOCLIP | DT_VCENTER);

  static char heightstr[10];
  static RECT heightrect;
  SetRect(&heightrect,462,9,516,28);
  sprintf(heightstr,"%.1f",(D3DXToDegree(((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight)));
  g_D3DObject->DrawTextStr_StatusBar(&heightrect,D3DCOLOR_XRGB(255,255,255),
                     heightstr, DT_RIGHT | DT_NOCLIP | DT_VCENTER);

  static char powerstr[10];
  static RECT powerrect;
  SetRect(&powerrect,543,9,589,28);
  sprintf(powerstr,"%.0f",(((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_firePower));
  g_D3DObject->DrawTextStr_StatusBar(&powerrect,D3DCOLOR_XRGB(255,255,255),
                     powerstr, DT_RIGHT | DT_VCENTER);

  static RECT missilerect;
  static char missilestr[50];
  SetRect(&missilerect,631,9,778,28);
  sprintf (missilestr,"%s %i",c3DObjectMissile::GetMissileStr(m_PlayerState[m_currentplayer].msl_cur_type),
    min(m_PlayerState[m_currentplayer].numweapons[m_PlayerState[m_currentplayer].msl_cur_type],9));
  g_D3DObject->DrawTextStr_StatusBar(&missilerect,D3DCOLOR_XRGB(255,255,255),
    missilestr, DT_CENTER | DT_VCENTER);
  }

}

void cGameState::GetInput(void)
{
  D3DXVECTOR3 maxis;
  D3DXVECTOR2 mscreen;

  g_D3DInput->MouseAxis(&maxis);        
  g_D3DInput->MouseScreen(&mscreen);

  c3DObjectTank *tmpP = NULL;

  static bool v_KEYUP_DOWN = true;
  static bool v_KEYUP_UP = true;
  static bool v_KEYUP_ENTER = true;

  static bool v_KEYUP_ESC = true;
  static bool v_KEYUP_PGUP = true;
  static bool v_KEYUP_PGDN = true;
  static bool v_KEYUP_F1 = true;
  static bool v_KEYUP_F2 = true;
  static bool v_KEYUP_Z = true;
  static bool v_KEYUP_A = true;

  static bool v_KEYUP_W = true;
  static bool v_MOUSEUP_0 = true;

  static bool WIREFRAME = false;

  static bool v_KEYUP_F = true;
  static bool v_KEYUP_M = true;

 // g_D3DInput->GetInput((cTerrain *)m_terrain);
  if (g_D3DInput->KeyDown(DIK_W) && v_KEYUP_W) {
    if (WIREFRAME = !WIREFRAME) 
      g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    else g_D3DObject->m_d3ddevice9->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID );
     v_KEYUP_W = false; 
   }
  if (g_D3DInput->KeyDown(DIK_F) && v_KEYUP_F) {
     sh_FPS = !sh_FPS;
     v_KEYUP_F = false; 
   }
  if (g_D3DInput->KeyDown(DIK_LMENU) && g_D3DInput->KeyDown(DIK_M) && v_KEYUP_M) {
     sh_MAP = !sh_MAP;
     v_KEYUP_M = false; 
   }
  if (!g_D3DInput->KeyDown(DIK_W)) v_KEYUP_W = true;
  if (!g_D3DInput->KeyDown(DIK_F)) v_KEYUP_F = true;

  switch (m_mainstate) {
    case MAINSTATES::ENDGAME:
     if ((g_D3DInput->MouseDown(0) && v_MOUSEUP_0) || 
         (g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC)) {
        wav->play(mnu_select);
        g_D3DInput->ResetMouseScreen();
        m_mainstate = MAINSTATES::MAINMENU;
        m_gstate = STATES::NOTHING;
        v_KEYUP_ESC = false;
        v_MOUSEUP_0 = false;
        aistarted=false;
        return;
      }
    case MAINSTATES::PRELEVEL:
      if (!aistarted)
      {
        for (int x=0 ; x < m_numplayers; x++)
          if (m_PlayerState[x].iscomputer)
            AI_PickWeapons();
        aistarted=true;
      }
      if(g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC) {
        wav->play(mnu_select);
        g_D3DInput->ResetMouseScreen();
        m_mainstate = MAINSTATES::MAINMENU;
        m_gstate = STATES::NOTHING;
        v_KEYUP_ESC = false;
        aistarted=false;
        return;
      }
     if (g_D3DInput->MouseDown(0) && v_MOUSEUP_0) {
      switch(m_prelevelbutt) {
       case PRELEVELBUTT::PL_WEAPONBUY:
         if (m_tmSpinner.CmpTime() && CanBuy(m_prelevelplayer,m_prelevelmsl))
          { 
            m_PlayerState[m_prelevelplayer].numweapons[m_prelevelmsl]++;
            m_PlayerState[m_prelevelplayer].money -= c3DObjectMissile::GetMissileCost(m_prelevelmsl);
            v_KEYUP_ESC = false;
            v_MOUSEUP_0 = false;
            wav->play(mnu_move);
          }
          break;
       case PRELEVELBUTT::PL_STARTLEVEL:
        wav->play(mnu_select);
        g_D3DObject->DefaultRenderState();
        m_mainstate = MAINSTATES::LEVEL;
        m_gstate = STATES::TARGETING;
        v_KEYUP_ESC = false;
        v_MOUSEUP_0 = false;
        aistarted=false;
        return;
      }
     }
      break;
    case MAINSTATES::POSTLEVEL:
      if (g_D3DInput->MouseDown(0) || g_D3DInput->KeyDown(DIK_RETURN) || g_D3DInput->KeyDown(DIK_ESCAPE))
      {
        wav->play(mnu_select);
        g_D3DObject->DefaultRenderState();
        v_KEYUP_ESC = false;
        v_MOUSEUP_0 = false;
        NextLevel();
        return;
      }
      break;
    case MAINSTATES::GAMESETUP:
      if(g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC) {
        wav->play(mnu_select);
        g_D3DInput->ResetMouseScreen();
        m_mainstate = MAINSTATES::MAINMENU;
        v_KEYUP_ESC = false;
      }
      if (g_D3DInput->MouseDown(0) && v_MOUSEUP_0) {
        switch (m_gamesetupbutt)
        {
        case GAMESETUPBUTT::GS_STARTGAME:
           _InitGame();
           break;
        case GAMESETUPBUTT::GS_CANCEL:
          // Goto Main Menu
          wav->play(mnu_select);
          g_D3DInput->ResetMouseScreen();
          m_mainstate = MAINSTATES::MAINMENU;
          v_MOUSEUP_0 = false;
        break;
        case GAMESETUPBUTT::GS_HUMANPL_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numHumans > 1))
          { 
            m_LevelState.numHumans--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_HUMANPL_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numHumans < m_LevelState.maxHumans)) {
            m_LevelState.numHumans++;
            wav->play(mnu_move);
          }
          break;

        case GAMESETUPBUTT::GS_CMPPL_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numComputers > 0)) {
            m_LevelState.numComputers--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_CMPPL_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numComputers < m_LevelState.maxComputers)) {
            m_LevelState.numComputers++;
            wav->play(mnu_move);
          }
          break;

        case GAMESETUPBUTT::GS_MONEY_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.startingMoney > 0)) {
            m_LevelState.startingMoney--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_MONEY_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.startingMoney < m_LevelState.maxMoney)) {
            m_LevelState.startingMoney++;
            wav->play(mnu_move);
          }
          break;

        case GAMESETUPBUTT::GS_HILLS_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numHills > 0)) {
            m_LevelState.numHills--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_HILLS_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numHills < m_LevelState.maxHills)) {
            m_LevelState.numHills++;
            wav->play(mnu_move);
          }
          break;

        case GAMESETUPBUTT::GS_WIND_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.windLevel > 0)) {
            m_LevelState.windLevel--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_WIND_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.windLevel < m_LevelState.maxWind)) {
            m_LevelState.windLevel++;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_DIRT_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numDirt > 0)) {
            m_LevelState.numDirt--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_DIRT_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numDirt < m_LevelState.maxDirt)) {
            m_LevelState.numDirt++;
            wav->play(mnu_move);
          }
          break;

        case GAMESETUPBUTT::GS_NLVL_DN:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numLevels > 1)) {
            m_LevelState.numLevels--;
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_NLVL_UP:
          if (m_tmSpinner.CmpTime() && (m_LevelState.numLevels < m_LevelState.maxLevels)) {
            m_LevelState.numLevels++;
            wav->play(mnu_move);
          }
          break;

        case GAMESETUPBUTT::GS_CMPAI_UP:
          if (m_tmSpinner.CmpTime()) {
            m_LevelState.aiComputers[m_LevelState.curComputer] = (st_LevelState::COMPUTER_AI)
              ((int)(m_LevelState.aiComputers[m_LevelState.curComputer] + 1) % (int)m_LevelState.AI_MAX);
            wav->play(mnu_move);
          }
          break;
        case GAMESETUPBUTT::GS_CMPAI_DN:
          if (m_tmSpinner.CmpTime()) {            
            m_LevelState.aiComputers[m_LevelState.curComputer] = (st_LevelState::COMPUTER_AI)
              ((int)(m_LevelState.aiComputers[m_LevelState.curComputer] - 1) % (int)m_LevelState.AI_MAX);
            if (m_LevelState.aiComputers[m_LevelState.curComputer] < 0)
              m_LevelState.aiComputers[m_LevelState.curComputer] = (st_LevelState::COMPUTER_AI) 
              ((int)m_LevelState.AI_MAX - 1);
            wav->play(mnu_move);
          }
          break;
        default: break;
        }
      }
      break;
    case MAINSTATES::HELP:
      if((g_D3DInput->MouseDown(0) && v_MOUSEUP_0) || 
         (g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC)) {
        wav->play(mnu_select);
        g_D3DInput->SetMouseScreen(D3DXVECTOR2(WIDTH/2,HEIGHT/2));
        SAFE_DELETE(m_helpBk);
        m_mainstate = MAINSTATES::MAINMENU;
        v_KEYUP_ESC = false;
        v_MOUSEUP_0 = false;
      }
      break;
    case MAINSTATES::CREDITS:
      if((g_D3DInput->MouseDown(0) && v_MOUSEUP_0) || 
         (g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC)) {
        wav->play(mnu_select);
        g_D3DInput->SetMouseScreen(D3DXVECTOR2(WIDTH/2,HEIGHT/2));
        SAFE_DELETE(m_creditsBk);
        m_mainstate = MAINSTATES::MAINMENU;
        v_KEYUP_ESC = false;
        v_MOUSEUP_0 = false;
      }
      break;
    case MAINSTATES::MAINMENU:
      if(g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC) {
         wav->play(mnu_select);
         SendMessage(g_hWnd,WM_QUIT,0,0);
         return;
      }
      if((g_D3DInput->MouseDown(0) && v_MOUSEUP_0)) {
           v_MOUSEUP_0 = false;
           v_KEYUP_ESC = false;
        switch (m_mainmenubutt) {
         case MAINMENUBUTT::MM_HELP:
           wav->play(mnu_select);
           m_mainstate = MAINSTATES::HELP;
           m_helpBk = new CTexture("resource\\pre-round\\helpscreen.png");
           break;
         case MAINMENUBUTT::MM_CREDITS:
         wav->play(mnu_select);
           m_mainstate = MAINSTATES::CREDITS;
           m_creditsBk = new CTexture("resource\\pre-round\\credits.png");
           break;
          break;
         case MAINMENUBUTT::MM_NEWGAME:
          wav->play(mnu_select);
          _InitLevelState();
          m_mainstate = MAINSTATES::GAMESETUP;
            //m_gstate = STATES::TARGETING;
          break;
         case MAINMENUBUTT::MM_QUIT:
           wav->play(mnu_select);
           SendMessage(g_hWnd,WM_QUIT,0,0);
         default: break;
        }
      }
      break;
    case MAINSTATES::LEVEL:
     if(g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC ) {
       wav->play(mnu_select);
       g_D3DInput->SetMouseScreen(D3DXVECTOR2(WIDTH/2,HEIGHT/2));
       m_gstate = STATES::NOTHING;
       m_mainstate = MAINSTATES::MAINMENU;
       g_ObjMgr->reset();
       m_numplayers = 0;
       g_D3DObject->DefaultRenderState();
       v_KEYUP_ESC = false;
       break;
     }
    switch (m_gstate) {
      case TARGETING:          
          if (m_PlayerState[m_currentplayer].iscomputer) {
            if (aistarted) {
              if (m_PlayerState[m_currentplayer].object->RotatedTo())
              {
                // Fire Missile
                aistarted = false;
                tmpP = (c3DObjectTank *)m_PlayerState[m_currentplayer].object;
                m_PlayerState[m_currentplayer].msl_object = (c3DObjectMissile *)
                  tmpP->Fire(m_PlayerState[m_currentplayer].msl_cur_type);
                SetCurrentCamera(&m_camBehindMissile);
                m_gstate = FIRING; 
                break;
              }
            }
            else {
              // Start AI
              aistarted = true;
              AI_TakeShot();
            }
            break;
          }         
          tmpP = (c3DObjectTank *)m_PlayerState[m_currentplayer].object;
          if (g_D3DInput->KeyDown(DIK_PGUP) && v_KEYUP_PGUP) {
            NextWeapon(-1); v_KEYUP_PGUP = false; 
          }
          if (g_D3DInput->KeyDown(DIK_PGDN) && v_KEYUP_PGDN) {
            NextWeapon(+1); v_KEYUP_PGDN = false; 
          }
          if (g_D3DInput->KeyDown(DIK_LMENU) && g_D3DInput->KeyDown(DIK_G))
          {
              for (int k = 0; k < c3DObjectMissile::MSLNUM; k++)
                  m_PlayerState[m_currentplayer].numweapons[k] = 99;
          }
          if (g_D3DInput->KeyDown(DIK_LMENU) && g_D3DInput->KeyDown(DIK_K))
          {
              for (int k = 0; k < m_numplayers; k++)
                  if (k != m_currentplayer) m_PlayerState[k].health = 0;
              KillDeadTanks();
              m_gstate = KILLDEADTANKS;
          }

          if (g_D3DInput->KeyDown(DIK_F1) && v_KEYUP_F1) {
              m_PlayerState[m_currentplayer].camabove = true;
              SetCurrentCamera(&m_camBehindTank);
              v_KEYUP_F1= false; 
          }

          if (g_D3DInput->KeyDown(DIK_F2) && v_KEYUP_F2) {
              m_PlayerState[m_currentplayer].camabove = false;
              SetCurrentCamera(&m_camAboveTerrain);
              v_KEYUP_F2= false; 
          }

          if (g_D3DInput->KeyDown(DIK_Z) && v_KEYUP_Z) {
              m_PlayerState[m_currentplayer].camabovezoom *= 0.80f;
              v_KEYUP_Z= false; 
          }

          if (g_D3DInput->KeyDown(DIK_A) && v_KEYUP_A) {
              m_PlayerState[m_currentplayer].camabovezoom *= 1.10f;
              v_KEYUP_A= false; 
          }

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
          if (g_D3DInput->KeyDown(DIK_SPACE) || (g_D3DInput->MouseDown(0) && v_MOUSEUP_0)) {
            m_PlayerState[m_currentplayer].msl_object = (c3DObjectMissile *)tmpP->Fire(m_PlayerState[m_currentplayer].msl_cur_type);          
            SetCurrentCamera(&m_camBehindMissile);
            v_MOUSEUP_0 = false;
            m_gstate = FIRING;
          }
          
          if (fabs(maxis.x) > 0.05)

            tmpP->event(c3DObjectTank::EVENT::RIGHT,fabs(maxis.x/10) < 80 ? maxis.x/10 : maxis.x/-maxis.x*80 );
          if (fabs(maxis.y) > 0.05)
            tmpP->event(c3DObjectTank::EVENT::DOWN,fabs(maxis.y/10) < 80 ? maxis.y/10 : maxis.y/-maxis.y*80 );
          if (fabs(maxis.z) > 0.05)
            tmpP->event(c3DObjectTank::EVENT::PWRDN,maxis.z/80);

          break;
      case FIRING:
        break;
        if (g_D3DInput->KeyDown(DIK_N))
        {
          m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
          SetCurrentCamera(&m_camBehindTank);
          m_gstate = TARGETING;
          aistarted = false;
        }
        break;
      default:
        break;
    }
    break;
    default:
      break;
  }
          
  if (!g_D3DInput->MouseDown(0)) {
    v_MOUSEUP_0 = true;
  }
          if (!g_D3DInput->KeyDown(DIK_ESCAPE))
            v_KEYUP_ESC = true;
          if (!g_D3DInput->KeyDown(DIK_PGUP))
            v_KEYUP_PGUP = true;
          if (!g_D3DInput->KeyDown(DIK_PGDN))
            v_KEYUP_PGDN = true;
          if (!g_D3DInput->KeyDown(DIK_F1))
            v_KEYUP_F1 = true;
          if (!g_D3DInput->KeyDown(DIK_F2))
            v_KEYUP_F2 = true;
          if (!g_D3DInput->KeyDown(DIK_Z))
            v_KEYUP_Z = true;
          if (!g_D3DInput->KeyDown(DIK_A))
            v_KEYUP_A = true;
          if (!g_D3DInput->KeyDown(DIK_M))
            v_KEYUP_M = true;
}

void cGameState::GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient)
{
  *pos =      ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position;
  orient->x = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate;
  orient->y = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight;
  orient->z = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

void cGameState::GetCurrentExpState(D3DXVECTOR3 *pos, D3DXVECTOR3 *scale, float *radius)
{
  *pos =    (m_PlayerState[m_currentplayer].exp_object)->m_position;
  *scale =  (m_PlayerState[m_currentplayer].exp_object)->m_scale;
  *radius = (m_PlayerState[m_currentplayer].exp_object)->m_radius;
//  orient->x = (m_PlayerState[m_currentplayer].object)->m_turretRotate;
//  orient->y = (m_PlayerState[m_currentplayer].object)->m_barrelHeight;
//  orient->z = (m_PlayerState[m_currentplayer].object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

void cGameState::GetCurrentMissileState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient, D3DXVECTOR3 *velocity)
{
  *pos         = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_position;
  *orient      = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_orient;
  *velocity    = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_velocity;
  //orient->y = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_barrelHeight;
  //orient->z = ((c3DObjectMissile *)m_PlayerState[m_currentplayer].msl_object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

cGameState::cGameState(void)
{
  m_numplayers=0;
  m_terrain = new cTerrain(1024,1024,20.0f);
  //m_terrain = new cTerrain(50,50,10.0f);
  m_skybox = new cSkyBox();
  m_tmissile = new c3DObjectMissile();
  m_mainstate = MAINSTATES::MAINMENU;
  m_gstate = STATES::NOTHING;
  SetCurrentCamera(&m_camera);
  m_tmSpinner.setInterval(400.0f);
  //m_terrain->RandomizeMesh();
  _Init();
}

cGameState::~cGameState(void)
{
  SAFE_DELETE(m_terrain);  
  SAFE_DELETE(m_skybox);
  SAFE_DELETE(m_tmissile);
  _Delete();
}


void cGameState::AddPlayer_old(BOOL human)
{
  c3DObject *tmpP = NULL;
  m_currentplayer = 0;

  m_gstate = cGameState::TARGETING;

  srand(150);
  m_terrain->RandomizeTerrain(40,5000);
  float t_x, t_y, t_z;

  for (int x =0 ; x < 5; x++)  {
    tmpP = new c3DObjectTank();
    t_x = (float)(rand()%300-150);
    t_z = (float)(rand()%300-150);
    m_terrain->FlattenSquare(t_x,t_z,c3DObjectTank::tank_width*2.1f);
    t_y = m_terrain->GetHeight(t_x,t_z)+c3DObjectTank::tank_height;
    tmpP->pos(D3DXVECTOR3((float)t_x,(float)t_y,(float)t_z));
    ((c3DObjectTank *)tmpP)->skin((c3DObjectTank::SKINS)x);
    m_PlayerState[m_numplayers].object = (c3DObjectTank *)tmpP;
    g_ObjMgr->add(m_PlayerState[m_numplayers].object);  
    m_PlayerState[x].health = 100.0f;
    m_PlayerState[x].money = 1234567890.0f;
    m_PlayerState[x].name = new char[50];
    m_PlayerState[x].livingstate = ALIVE;
    m_PlayerState[x].msl_cur_type = (c3DObjectMissile::MSLTYPE::SHELL);
    m_PlayerState[x].camabove = false;
    m_PlayerState[x].camabovezoom = 1.0f;
    sprintf (m_PlayerState[x].name,"%s%d","Player ",x);

    for (int k = 0; k < c3DObjectMissile::MSLNUM; k++)
      m_PlayerState[x].numweapons[k] = 30;
    m_numplayers++;
  }

  SetCurrentCamera(&m_camBehindTank);
}

void cGameState::AddPlayer(BOOL human)
{
  c3DObject *tmpP = NULL;

//  float t_x, t_z, t_y;

  tmpP = new c3DObjectTank();
  // Choose a random position

  ((c3DObjectTank *)tmpP)->skin((c3DObjectTank::SKINS)(rand()%5));
   m_PlayerState[m_numplayers].object = (c3DObjectTank *)tmpP;
   g_ObjMgr->add(m_PlayerState[m_numplayers].object);  
   m_PlayerState[m_numplayers].money = m_LevelState.startingMoney * 5.14f * 1000000;   // Fix
   m_PlayerState[m_numplayers].name = new char[50];
   if (human) {
     sprintf (m_PlayerState[m_numplayers].name,"%s%d","Human ",m_LevelState.numHumansTMP+1);
     m_PlayerState[m_numplayers].iscomputer = false;
     m_LevelState.numHumansTMP++;
   }
   else {
     if (m_LevelState.aiComputers[m_LevelState.numComputersTMP] == st_LevelState::COMPUTER_AI::RANDOM)
       m_PlayerState[m_numplayers].ai_type = (st_LevelState::COMPUTER_AI)(rand()%4);
     else
       m_PlayerState[m_numplayers].ai_type = m_LevelState.aiComputers[m_LevelState.numComputersTMP];
     m_PlayerState[m_numplayers].iscomputer = true;
     sprintf (m_PlayerState[m_numplayers].name,"Comp %d",m_LevelState.numComputersTMP+1);
     m_LevelState.numComputersTMP++;
   }
  for (int k = 0; k < c3DObjectMissile::MSLNUM; k++)
      m_PlayerState[m_numplayers].numweapons[k] = 0;
  m_PlayerState[m_numplayers].numweapons[c3DObjectMissile::SHELL] = 99;
  m_numplayers++;
}

cCamera * cGameState::GetCurrentCamera() { 
  return m_curcamera; 
}

void cGameState::_Init(void)
{
  /*
    if (FAILED(D3DXCreateTextureFromFileEx(
                                  g_D3DObject->m_d3ddevice9,    // Device
                                  "resource\\statusbar\\statusbar.dds",
                                  D3DX_DEFAULT,D3DX_DEFAULT,    // Height/Width
                                  D3DX_DEFAULT,                 // MIP Maps
                                  0,                            // Usage
                                  D3DFMT_A8R8G8B8,              // Format
                                  D3DPOOL_DEFAULT,              // Pool
                                  D3DX_DEFAULT,                 // Filter
                                  D3DX_DEFAULT,                 // MIP Filter (Default = BOX)
                                  0,                            // Color Key (0 = disabled)
                                  NULL,                         // Ptr to SRCINFO
                                  NULL,                         // Palette entry
                                  &m_statusbartex               // Ptr to Texture
                                  )))
                                  exit(0);
    */
  RECT imgrect;    
    
    m_statusbartex = new CTexture("resource\\statusbar\\statusbar.dds");
    m_preroundBk = new CTexture("resource\\pre-round\\roundbackground.png");
    SetRect(&imgrect,1,1,35,35);
    m_preroundMousePtr = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);

    SetRect(&imgrect,1,37,401,81);
    m_preroundRegButton[1] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
    SetRect(&imgrect,1,84,401,128);
    m_preroundRegButton[0] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);

    SetRect(&imgrect,1,232,130,277);
    m_preroundSmButton[1] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
    SetRect(&imgrect,1,279,130,323);
    m_preroundSmButton[0] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);

    SetRect(&imgrect,1,130,385,230);
    m_preroundLogo = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);

    SetRect(&imgrect,37,1,71,35);
    m_spinner[0] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
    SetRect(&imgrect,73,1,107,35);
    m_spinner[1] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
    SetRect(&imgrect,109,1,143,35);
    m_spinner[2] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);

    SetRect(&imgrect,145,1,255,35);
    m_textfield[0] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
}

void cGameState::_Delete(void)
{
  SAFE_DELETE(m_statusbartex);
  SAFE_DELETE(m_preroundBk);
  SAFE_DELETE(m_preroundMousePtr);
  SAFE_DELETE(m_preroundRegButton[0]);
  SAFE_DELETE(m_preroundRegButton[1]);
  SAFE_DELETE(m_preroundLogo);

  SAFE_DELETE(m_spinner[0]);
  SAFE_DELETE(m_spinner[1]);
  SAFE_DELETE(m_spinner[2]);
  SAFE_DELETE(m_textfield[0]);

}

void cGameState::OnLostDevice() {
  m_skybox->OnLostDevice();
  m_terrain->OnLostDevice();
  _Delete();
}

void cGameState::OnResetDevice() {
  m_skybox->OnResetDevice();
  m_terrain->OnResetDevice();
  _Init();
}

void cGameState::AssignHits()
{
   for (int x = 0; x < m_numplayers; x++)
   {
     D3DXVECTOR3 dis = m_PlayerState[x].object->m_position - m_PlayerState[m_currentplayer].exp_object->m_position;
     float rad_diff = m_PlayerState[m_currentplayer].exp_object->m_radius - D3DXVec3Length(&dis);
     if (rad_diff > 0) {
       float dmg;
       if (rad_diff < m_PlayerState[m_currentplayer].exp_object->m_radius/3) dmg = m_PlayerState[m_currentplayer].exp_object->m_radius/0.8;
       else if (rad_diff < 2*m_PlayerState[m_currentplayer].exp_object->m_radius/3) dmg = m_PlayerState[m_currentplayer].exp_object->m_radius/1.0;
       else dmg = m_PlayerState[m_currentplayer].exp_object->m_radius/2.0;
       m_PlayerState[x].health -= dmg;
       m_PlayerState[m_currentplayer].numDamage += dmg;
       m_PlayerState[m_currentplayer].money += dmg*15000;
       m_LevelState.numDamage[m_currentplayer] += dmg;
       m_LevelState.numMoney[m_currentplayer] += dmg*15000;
     }
   }
}

void cGameState::DropTanks()
{
   for (int x = 0; x < m_numplayers; x++)
   {
     float pl_x = m_PlayerState[x].object->m_position.x;
     float pl_z = m_PlayerState[x].object->m_position.z;

     float mod_terheight = m_terrain->GetHeight(pl_x,pl_z) +  c3DObjectTank::tank_height;

     if ((mod_terheight+0.2f) < m_PlayerState[x].object->m_position.y)
     {
       //OutputDebugString("Drop tanks registered it on ");
       //OutputDebugString(m_PlayerState[x].name);
       //OutputDebugString("\n");
       m_terrain->FlattenSquare(pl_x,pl_z,c3DObjectTank::tank_width*2.1f);
       // Get Damage
       float dmg = min((m_PlayerState[x].object->m_position.y - m_terrain->GetHeight(pl_x,pl_z)),30);
       m_PlayerState[x].health -= dmg;
       // Record Damage Credit
       m_LevelState.numDamage[m_currentplayer] += dmg;
       m_LevelState.numMoney[m_currentplayer] += dmg*10000;
       m_PlayerState[m_currentplayer].numDamage+= dmg;
       m_PlayerState[m_currentplayer].money += dmg*10000;

       // Move tank down!!!
       m_PlayerState[x].object->m_position.y = m_terrain->GetHeight(pl_x,pl_z) + c3DObjectTank::tank_height;
     }
   }
}

void cGameState::KillDeadTanks()
{
   for (int x = 0; x < m_numplayers; x++)
     if (m_PlayerState[x].health <= 0.0f && m_PlayerState[x].livingstate == ALIVE)
     {
       m_LevelState.numKills[m_currentplayer]++;
       m_PlayerState[m_currentplayer].numKills++;
       m_LevelState.numMoney[m_currentplayer] += 12000000;
       m_PlayerState[m_currentplayer].numKills++;
       m_PlayerState[m_currentplayer].money += 12000000;
       m_PlayerState[x].livingstate = DYING;
     }
}

void cGameState::NextWeapon(int t_dir)
{
  m_PlayerState[m_currentplayer].numweapons[(int)c3DObjectMissile::MSLTYPE::SHELL] = 99;
  m_PlayerState[m_currentplayer].msl_cur_type = (c3DObjectMissile::MSLTYPE)
       ((m_PlayerState[m_currentplayer].msl_cur_type + t_dir) % c3DObjectMissile::MSLNUM);
  if (m_PlayerState[m_currentplayer].msl_cur_type < 0) 
    m_PlayerState[m_currentplayer].msl_cur_type = (c3DObjectMissile::MSLTYPE)(m_PlayerState[m_currentplayer].msl_cur_type +
          c3DObjectMissile::MSLNUM);
  while (m_PlayerState[m_currentplayer].numweapons[m_PlayerState[m_currentplayer].msl_cur_type]<=0)
  {
   m_PlayerState[m_currentplayer].msl_cur_type = (c3DObjectMissile::MSLTYPE)
       ((m_PlayerState[m_currentplayer].msl_cur_type + t_dir) % c3DObjectMissile::MSLNUM);
   if (m_PlayerState[m_currentplayer].msl_cur_type < 0) 
    m_PlayerState[m_currentplayer].msl_cur_type = (c3DObjectMissile::MSLTYPE)(m_PlayerState[m_currentplayer].msl_cur_type +
          c3DObjectMissile::MSLNUM);
  }
}

void cGameState::GetCurrentCamAboveZoom(float *flt)
{
  *flt = m_PlayerState[m_currentplayer].camabovezoom;
}

void cGameState::_InitLevelState(void)
{
   m_LevelState.numHumans = 1;
   m_LevelState.numComputers = 1;
   m_LevelState.numHills = 1;
   m_LevelState.numDirt = 1;
   m_LevelState.startingMoney = 5;
   m_LevelState.windLevel = 1;
   m_LevelState.numLevels = 10;

   for (int x = 0 ; x < st_LevelState::maxComputers; x++)
   {
     m_LevelState.aiComputers[x] = st_LevelState::COMPUTER_AI::RANDOM;
   }
}

char * strComputer_AI(enum st_LevelState::COMPUTER_AI ai_in)
{
   switch (ai_in)
   {
   case st_LevelState::COMPUTER_AI::SEEKER: return "Seeker"; break;
   case st_LevelState::COMPUTER_AI::MEAN: return "Mean"; break;
   case st_LevelState::COMPUTER_AI::NOVICE: return "Novice"; break;
   case st_LevelState::COMPUTER_AI::EXPERT: return "Expert"; break;
   case st_LevelState::COMPUTER_AI::RANDOM: return "Random"; break;
   default: return "Unknown"; break;
   }
}

char * strLevel_WIND(int wind)
{
   switch (wind)
   {
   case 0: return "Stale"; break;
   case 1: return "Calm"; break;
   case 2: return "Windy"; break;
   case 3: return "Brisk"; break;
   case 4: return "Tornado"; break;
   default: return "Unknown"; break;
   }
}
char * strLevel_MONEY(int money)
{
  float fmoney = money * 5.14f;
  static char moneybuff[65];
  sprintf(moneybuff,"%.2f M",fmoney);
  return moneybuff;
}

char * strLevel_HILLS(int hills)
{
   switch (hills)
   {
   case 0: return "None"; break;
   case 1: return "Few"; break;
   case 2: return "Sparse"; break;
   case 3: return "Many"; break;
   case 4: return "Google"; break;
   default: return "Unknown"; break;
   }
}
char * strLevel_DIRT(int dirt)
{
   switch (dirt)
   {
   case 0: return "None"; break;
   case 1: return "C'Mon!"; break;
   case 2: return "Sparse"; break;
   case 3: return "Spew"; break;
   case 4: return "Chunks"; break;
   default: return "Unknown"; break;
   }
}

void cGameState::_InitGame(void)
{
  srand(timeGetTime());
  RoundNumber = 0;
  m_numplayers = 0;
  m_newwintext = true;

  m_LevelState.numHumansTMP = 0;
  m_LevelState.numComputersTMP = 0;

  while ((m_LevelState.numComputersTMP < m_LevelState.numComputers) || 
    (m_LevelState.numHumansTMP < m_LevelState.numHumans))
  {
    int rndnum;
    rndnum = rand() % 100;
    if (rndnum > 50)  if ((m_LevelState.numComputersTMP < m_LevelState.numComputers)) AddPlayer(false);
    if (rndnum < 30)  if ((m_LevelState.numHumansTMP < m_LevelState.numHumans))  AddPlayer(true);
  }

  NextLevel();
}
void cGameState::NextLevel(void)
{
  if (RoundNumber++ >= m_LevelState.numLevels) {
//    g_ObjMgr->reset();
    m_mainstate = MAINSTATES::ENDGAME;
    m_gstate = STATES::NOTHING;
    return;
  }
  aistarted = false;
  CTimer::Pause();
  srand(timeGetTime());

  int numHills = 4 + (m_LevelState.numHills*23) + rand()%6;
  int numDirt = 1000 + (m_LevelState.numDirt*10000) + rand()%250;
  
  m_terrain->RandomizeTerrain(numHills,numDirt);
  m_terrain->RandomizeEnvironment();
  m_skybox->RandomizeSky();

//  g_ObjMgr->reset();
  for (int d = 0; d < m_numplayers; d++)
  {
    float t_x = (float)(rand()%299-148);
    float t_z = (float)(rand()%299-148);
    m_terrain->FlattenSquare(t_x,t_z,c3DObjectTank::tank_width*2.1f);
    float t_y = m_terrain->GetHeight(t_x,t_z)+c3DObjectTank::tank_height;
    m_PlayerState[d].object->ResetLevel();
    m_PlayerState[d].object->pos(D3DXVECTOR3((float)t_x,(float)t_y,(float)t_z));
    m_PlayerState[d].health = 100;
    m_PlayerState[d].livingstate = ALIVE;
    m_PlayerState[d].camabove = false;
    m_PlayerState[d].camabovezoom = 1.0f;
    m_PlayerState[d].msl_cur_type = (c3DObjectMissile::MSLTYPE::SHELL);
    m_PlayerState[d].camabove = false;
    m_PlayerState[d].camabovezoom = 1.0f;

    m_LevelState.numScore[d] = 0;
    m_LevelState.numMoney[d] = 0.0f;
    m_LevelState.numDamage[d] = 0.0f;
    m_LevelState.numKills[d] = 0;
  }

  m_currentplayer = rand() % m_numplayers;
  SetCurrentCamera(&m_camBehindTank);
  m_mainstate = MAINSTATES::PRELEVEL;
  m_gstate = STATES::NOTHING;
  CTimer::UnPause();
}

int cGameState::returnplayers()
{   return m_numplayers;
}

int cGameState::returncurrentplayer()
{	return m_currentplayer;
}

void cGameState::UpdateScores()
{
  for (int x =0; x < m_numplayers; x++)
  {
    m_LevelState.numScore[x] = (m_LevelState.numKills[x] * 8.5 + m_LevelState.numDamage[x]/100.0f);
    m_PlayerState[x].score += m_LevelState.numScore[x];
  }
}

void cGameState::AI_TakeShot()
{
    float rot;
    float turret;
    float power;
    D3DXVECTOR3 dist;
 switch (m_PlayerState[m_currentplayer].ai_type)
  {
  case st_LevelState::COMPUTER_AI::SEEKER: 
  case st_LevelState::COMPUTER_AI::MEAN:
  case st_LevelState::COMPUTER_AI::EXPERT:
    // Choose a target
    for (int x = 0; x < rand()%50; x++)
      NextWeapon(1);
    int target;
    target = -1;
    while (target == -1)
    {
      int t = rand()%m_numplayers;
      if ((t != m_currentplayer) && (m_PlayerState[t].livingstate == ALIVE))
        target = t;
    }

    dist = m_PlayerState[m_currentplayer].object->m_position -
                       m_PlayerState[target].object->m_position;
    rot = dist.z == 0 ? 0 : atan(dist.x/dist.z);
    rot += (rand()%20)/20-0.5f;
    turret = D3DX_PI/16 + fmod (rand(),D3DX_PI/2 - D3DX_PI/8);
    power = 100+rand()%900;
    power += (rand()%1000)/50-100;
    m_PlayerState[m_currentplayer].object->RotateTo(rot,turret,power);
    break;
  case st_LevelState::COMPUTER_AI::RANDOM:
  case st_LevelState::COMPUTER_AI::NOVICE:
  default: 
    NextWeapon(1);
    rot = (rand()%(int)(D3DX_PI*2*1000))/1000;
    turret = D3DX_PI/16 + fmod (rand(),D3DX_PI/2 - D3DX_PI/8);
    power = (rand()%1900)/2+100;
    m_PlayerState[m_currentplayer].object->RotateTo(rot,turret,power);
    break;
  }
}

void cGameState::AI_PickWeapons()
{
  switch (m_PlayerState[m_currentplayer].ai_type)
  {
  case st_LevelState::COMPUTER_AI::SEEKER: 
    for (int x = 0; x < 100; x++)
    {
      if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::FUNKIEBOMB) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::FUNKIEBOMB]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::FUNKIEBOMB) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::FUNKIEBOMB]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::SCUD) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::SCUD]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::SCUD) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::SCUD]++;
      }
    }
  break;
  case st_LevelState::COMPUTER_AI::MEAN:
    for (int x = 0; x < 100; x++) {
      if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::SCUD) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::SCUD]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::FUNKIEBOMB) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::FUNKIEBOMB]++;
      }
  }
  case st_LevelState::COMPUTER_AI::EXPERT:
    for (int x = 0; x < 100; x++)
    {
      if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::ATOMBOMB) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::ATOMBOMB]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::FUNKIEBOMB) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::FUNKIEBOMB]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::AMRAM) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::AMRAM]++;
      }
      else if (float cst = c3DObjectMissile::GetMissileCost(c3DObjectMissile::MSLTYPE::SCUD) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[c3DObjectMissile::MSLTYPE::SCUD]++;
      }
    }
    break;
  case st_LevelState::COMPUTER_AI::RANDOM:
  case st_LevelState::COMPUTER_AI::NOVICE:
  default: 
    for (int x = 0; x < 100; x++)
    {
      int msl_type = rand()%c3DObjectMissile::MSLNUM;
      if (float cst = c3DObjectMissile::GetMissileCost((c3DObjectMissile::MSLTYPE) msl_type) < 
        m_PlayerState[m_currentplayer].money)
      {
        m_PlayerState[m_currentplayer].money-= cst;
        m_PlayerState[m_currentplayer].numweapons[(c3DObjectMissile::MSLTYPE) msl_type]++;
      }

    }
    break;
  }
}

BOOL cGameState::CanBuy(int plyr, c3DObjectMissile::MSLTYPE msl)
{
  if (m_PlayerState[plyr].money >= c3DObjectMissile::GetMissileCost(msl))
    return true;
  else
    return false;
}

char *cGameState::randomwintxt(void)
{
  static int rndnum;
  if (m_newwintext) {
    rndnum = rand()%5;
    m_newwintext = false;
  }
  switch (rndnum)
  {
  case 1: return "We have a weiner!"; break;
  case 2: return "Congratulations!"; break;
  case 3: return "Definitely not a loser!"; break;
  case 4: return "Long live the king!"; break;
  default: return "..And the winner is"; break;
  }
}
char *cGameState::getwinnername(void)
{
  int winner = 0;
  for (int x = 1; x < m_numplayers; x++)
    if (m_PlayerState[x].score > m_PlayerState[winner].score)
      winner = x;
  return m_PlayerState[winner].name;
}