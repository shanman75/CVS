#include "StdAfx.h"
#include "gamestate.h"
#include "MainWnd.h"
#include <stdio.h>
cGameState *g_GameState;
//LPDIRECT3DTEXTURE9 cGameState::m_statusbartex = NULL;
CTexture *cGameState::m_statusbartex = NULL;
CTexture *cGameState::m_preroundBk = NULL;
CTexture *cGameState::m_preroundMousePtr =NULL;
CTexture *cGameState::m_preroundRegButton[2];
CTexture *cGameState::m_preroundLogo = NULL;
CTexture *cGameState::m_helpBk = NULL;

CTexture *cGameState::m_creditsBk = NULL;

extern bool sh_FPS;

void cGameState::move(void)
{
  static CTimer tmrState;
  D3DXVECTOR3 msl_pos;
//  c3DObject *tmpP;
//  m_gstate = TARGETING;

  switch (m_gstate) 
  {
  case FIRING:
         msl_pos = m_PlayerState[m_currentplayer].msl_object->m_position;
         if (m_terrain->GetHeight(msl_pos.x,msl_pos.z) > msl_pos.y ||
             fabs(msl_pos.x) > 600.0f ||
             fabs(msl_pos.z) > 600.0f) {
           // Create New Explosion
           m_PlayerState[m_currentplayer].exp_object = 
             new c3DObjectExplosion(
                    c3DObjectMissile::GetMissileExpRadius(m_PlayerState[m_currentplayer].msl_cur_type),
                    4300.0f,
                    c3DObjectMissile::GetMissileExpColor(m_PlayerState[m_currentplayer].msl_cur_type)
                    );
           m_PlayerState[m_currentplayer].exp_object->pos(msl_pos);
           m_PlayerState[m_currentplayer].exp_object->m_position.y = m_terrain->GetHeight(
                        m_PlayerState[m_currentplayer].exp_object->m_position.x,
                        m_PlayerState[m_currentplayer].exp_object->m_position.z );
           g_ObjMgr->del(m_PlayerState[m_currentplayer].msl_object);
           g_ObjMgr->add(m_PlayerState[m_currentplayer].exp_object);
           m_PlayerState[m_currentplayer].msl_object = NULL;
           SetCurrentCamera(&m_camAboveExplosion);
           m_gstate = EXPLODING;
           tmrState.setInterval(8000);
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
          NextPlayer();
          if (m_PlayerState[m_currentplayer].camabove) 
            SetCurrentCamera(&m_camAboveTerrain);
          else
            SetCurrentCamera(&m_camBehindTank);
          m_gstate = TARGETING;
    }
    break;
  default:
    break;
  }
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
    if (but_ng == 0) m_mainmenubutt = MAINMENUBUTT::MM_NONE;
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
  else {
    m_skybox->Paint();
    m_terrain->Paint();
  }
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
  if (!g_D3DInput->KeyDown(DIK_W)) v_KEYUP_W = true;
  if (!g_D3DInput->KeyDown(DIK_F)) v_KEYUP_F = true;

  switch (m_mainstate) {
    case MAINSTATES::CREDITS:
      if((g_D3DInput->MouseDown(0) && v_MOUSEUP_0) || 
         (g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC)) {
        SAFE_DELETE(m_creditsBk);
        m_mainstate = MAINSTATES::MAINMENU;
        v_KEYUP_ESC = false;
        v_MOUSEUP_0 = false;
      }
      break;
    case MAINSTATES::MAINMENU:
      if(g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC) {
               SendMessage(g_hWnd,WM_QUIT,0,0);
               return;
      }
      if(g_D3DInput->MouseDown(0) && v_MOUSEUP_0) {
        switch (m_mainmenubutt) {
         case MAINMENUBUTT::MM_CREDITS:
           m_mainstate = MAINSTATES::CREDITS;
           m_creditsBk = new CTexture("resource\\pre-round\\credits.png");
           v_MOUSEUP_0 = false;
           v_KEYUP_ESC = false;
           break;
          break;
         case MAINMENUBUTT::MM_NEWGAME:
          CTimer::Pause();
          AddPlayer();
          CTimer::UnPause();
          m_mainstate = MAINSTATES::LEVEL;
          m_gstate = STATES::TARGETING;
          break;
         case MAINMENUBUTT::MM_QUIT:
           SendMessage(g_hWnd,WM_QUIT,0,0);
         default: break;
        }
      }
      break;
    case MAINSTATES::LEVEL:
     if(g_D3DInput->KeyDown(DIK_ESCAPE) && v_KEYUP_ESC ) {
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
          tmpP = (c3DObjectTank *)m_PlayerState[m_currentplayer].object;
          if (g_D3DInput->KeyDown(DIK_PGUP) && v_KEYUP_PGUP) {
            NextWeapon(-1); v_KEYUP_PGUP = false; 
          }
          if (g_D3DInput->KeyDown(DIK_PGDN) && v_KEYUP_PGDN) {
            NextWeapon(+1); v_KEYUP_PGDN = false; 
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
          if (g_D3DInput->KeyDown(DIK_SPACE) || g_D3DInput->MouseDown(0)) {
            m_PlayerState[m_currentplayer].msl_object = (c3DObjectMissile *)tmpP->Fire(m_PlayerState[m_currentplayer].msl_cur_type);          
            SetCurrentCamera(&m_camBehindMissile);
            m_gstate = FIRING;
          }
          
          if (fabs(maxis.x) > 0.10)
            tmpP->event(c3DObjectTank::EVENT::RIGHT,maxis.x/70);
          if (fabs(maxis.y) > 0.10)
            tmpP->event(c3DObjectTank::EVENT::DOWN,maxis.y/70);
          if (fabs(maxis.z) > 0.10)
            tmpP->event(c3DObjectTank::EVENT::PWRDN,maxis.z/70);

          break;
      case FIRING:
        break;
        if (g_D3DInput->KeyDown(DIK_N))
        {
          m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
          SetCurrentCamera(&m_camBehindTank);
          m_gstate = TARGETING;
        }
        break;
      default:
        break;
    }
    break;
    default:
      break;
  }
          if (!g_D3DInput->MouseDown(0))
            v_MOUSEUP_0 = true;
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


void cGameState::AddPlayer(BOOL human)
{
  c3DObject *tmpP = NULL;
  m_currentplayer = 0;

  m_gstate = cGameState::TARGETING;

  srand(150);
  m_terrain->RandomizeTerrain(5,500);
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
    m_preroundRegButton[0] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
    SetRect(&imgrect,1,84,401,128);
    m_preroundRegButton[1] = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);

    SetRect(&imgrect,1,130,385,230);
    m_preroundLogo = new CTexture("resource\\pre-round\\buttons.png",D3DXCOLOR(1.0f,0.0f,1.0f,1.0f),&imgrect);
}

void cGameState::_Delete(void)
{
  SAFE_DELETE(m_statusbartex);
  SAFE_DELETE(m_preroundBk);
  SAFE_DELETE(m_preroundMousePtr);
  SAFE_DELETE(m_preroundRegButton[0]);
  SAFE_DELETE(m_preroundRegButton[1]);
  SAFE_DELETE(m_preroundLogo);
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
     m_PlayerState[m_currentplayer].health -= max(0,m_PlayerState[m_currentplayer].exp_object->m_radius*
                         (m_PlayerState[m_currentplayer].exp_object->m_radius - D3DXVec3Length(&dis)));
   }
}

void cGameState::DropTanks()
{
   for (int x = 0; x < m_numplayers; x++)
   {
     float pl_x = m_PlayerState[x].object->m_position.x;
     float pl_z = m_PlayerState[x].object->m_position.z;

     float mod_terheight = m_terrain->GetHeight(pl_x,pl_z) +  c3DObjectTank::tank_height;

     if (mod_terheight < m_PlayerState[x].object->m_position.y)
     {
       //OutputDebugString("Drop tanks registered it on ");
       //OutputDebugString(m_PlayerState[x].name);
       //OutputDebugString("\n");
       m_terrain->FlattenSquare(pl_x,pl_z,c3DObjectTank::tank_width*2.1f);
       // Assign Damage
       m_PlayerState[x].health -= min((m_PlayerState[x].object->m_position.y - m_terrain->GetHeight(pl_x,pl_z)),10);
       // Move tank down!!!
       m_PlayerState[x].object->m_position.y = m_terrain->GetHeight(pl_x,pl_z) + c3DObjectTank::tank_height;
     }
   }
}

void cGameState::KillDeadTanks()
{
   for (int x = 0; x < m_numplayers; x++)
     if (m_PlayerState[x].health <= 0.0f)
       m_PlayerState[x].livingstate = DEAD;
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