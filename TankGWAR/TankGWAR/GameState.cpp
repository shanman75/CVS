#include "StdAfx.h"
#include "gamestate.h"
#include <stdio.h>
cGameState *g_GameState;
//LPDIRECT3DTEXTURE9 cGameState::m_statusbartex = NULL;
CTexture *cGameState::m_statusbartex = NULL;
void cGameState::move(void)
{
  static CTimer tmrState;
  D3DXVECTOR3 msl_pos;
  c3DObject *tmpP;

  switch (m_gstate) 
  {
  case FIRING:
         msl_pos = m_PlayerState[m_currentplayer].msl_object->m_position;
         if (m_terrain->GetHeight(msl_pos.x,msl_pos.z) > msl_pos.y) {
           // Create New Explosion
           m_PlayerState[m_currentplayer].exp_object = new c3DObjectExplosion();
           m_PlayerState[m_currentplayer].exp_object->pos(msl_pos);
           m_PlayerState[m_currentplayer].exp_object->m_position.y = m_terrain->GetHeight(
                        m_PlayerState[m_currentplayer].exp_object->m_position.x,
                        m_PlayerState[m_currentplayer].exp_object->m_position.z );
           g_ObjMgr->del(m_PlayerState[m_currentplayer].msl_object);
           g_ObjMgr->add(m_PlayerState[m_currentplayer].exp_object);
           m_PlayerState[m_currentplayer].msl_object = NULL;
           SetCurrentCamera(&m_camAboveExplosion);
           m_gstate = EXPLODING;
           tmrState.setInterval(6000);
           tmrState.Reset();
         }
    break;
  case EXPLODING:
    if (tmrState.CmpTime()) {
          g_ObjMgr->del(m_PlayerState[m_currentplayer].exp_object);
           SetCurrentCamera(&m_camBehindTank);
          m_currentplayer = (m_currentplayer + 1 ) % m_numplayers;
          m_gstate = TARGETING;
    }
    break;
  default:
    break;
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


  if (0) {
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
/*
  D3DXVECTOR2 mytrans= D3DXVECTOR2(90,90);
//  g_D3DObject->m_pd3dxSprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE|D3DXSPRITE_DONOTSAVESTATE);
   g_D3DObject->m_pd3dxSprite->Begin(D3DXSPRITE_DONOTSAVESTATE);
   D3DXMATRIXA16 transform;
   D3DXMatrixTransformation2D(&transform,
	   NULL,          // Scaling Center D3DXVECTOR2 
	   NULL,            // Scaling Rotation factor
	   NULL,           // Scaling Matrix  D3DXVECTOR2 
	   NULL,           // Rotation Center D3DXVECTOR2
	   NULL,			   // Rotation (in radians)
	  &mytrans           // Translation D3DXVECTOR2
	   );

  //g_D3DObject->m_pd3dxSprite->SetTransform(&transform);
  g_D3DObject->m_pd3dxSprite->Draw(
                          m_statusbartex,
                          NULL,   // Rectangle
                          NULL,   // Center
                          NULL,   // Position
                          0xFFFFFFFF    // Color
                          );
*/
  g_D3DObject->m_pd3dxSprite->Begin(0);
  m_statusbartex->Paint(0.0f,0.0f);
  g_D3DObject->m_pd3dxSprite->End();

  static RECT healthrect;
  SetRect(&healthrect,28,9,66,28);
  static char healthstr[255];
  sprintf(healthstr,"%.0f\%",m_PlayerState[m_currentplayer].health);
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
  SetRect(&missilerect,631,9,778,28);
  g_D3DObject->DrawTextStr_StatusBar(&missilerect,D3DCOLOR_XRGB(255,255,255),
    c3DObjectMissile::GetMissileStr(m_PlayerState[m_currentplayer].msl_cur_type), DT_CENTER | DT_VCENTER);

}
void cGameState::GetInput(void)
{
  c3DObjectTank *tmpP = NULL;

  g_D3DInput->GetInput((cTerrain *)m_terrain);

  switch (m_gstate) {
     case TARGETING:
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
          m_PlayerState[m_currentplayer].msl_object = (c3DObjectMissile *)tmpP->Fire(c3DObjectTank::MISSILE);          
          SetCurrentCamera(&m_camBehindMissile);
          m_gstate = FIRING;
        }
        break;
     case FIRING:
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
}

void cGameState::GetCurrentTankState(D3DXVECTOR3 *pos, D3DXVECTOR3 *orient)
{
  *pos =      ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_position;
  orient->x = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_turretRotate;
  orient->y = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_barrelHeight;
  orient->z = ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_firePower;
    //= ((c3DObjectTank *)m_PlayerState[m_currentplayer].object)->m_orient;
}

void cGameState::GetCurrentExpState(D3DXVECTOR3 *pos, D3DXVECTOR3 *scale)
{
  *pos =    (m_PlayerState[m_currentplayer].exp_object)->m_position;
  *scale = (m_PlayerState[m_currentplayer].exp_object)->m_scale;
//  orient->x = (m_PlayerState[m_currentplayer].object)->m_turretRotate;
//  orient->y = (m_PlayerState[m_currentplayer].object)->m_barrelHeight;
//  orient->z = (m_PlayerState[m_currentplayer].object)->m_firePower;
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
  m_terrain = new cTerrain(1850,1850,20.0f);
  //m_terrain = new cTerrain(50,50,10.0f);
  m_skybox = new cSkyBox();
  //m_terrain->RandomizeMesh();
  SetCurrentCamera(new cCameraBehindTank);
  _Init();
}

cGameState::~cGameState(void)
{
  SAFE_DELETE(m_terrain);  
  SAFE_DELETE(m_skybox);
  _Delete();
}


void cGameState::AddPlayer(BOOL human)
{
  c3DObject *tmpP = NULL;
  m_currentplayer = 0;

  m_gstate = cGameState::TARGETING;

  float t_x, t_y, t_z;

  for (int x =0 ; x < 5; x++)  {
    tmpP = new c3DObjectTank();
    t_x = rand()%600-300;
    t_z = rand()%600-300;
    m_terrain->FlattenSquare(t_x,t_z,14.0f);
    t_y = m_terrain->GetHeight(t_x,t_z)+2.0f;
    tmpP->pos(D3DXVECTOR3((float)t_x,(float)t_y,(float)t_z));
    ((c3DObjectTank *)tmpP)->skin((c3DObjectTank::SKINS)x);
    m_PlayerState[m_numplayers].object = (c3DObjectTank *)tmpP;
    g_ObjMgr->add(m_PlayerState[m_numplayers].object);  
    m_PlayerState[x].health = 100.0f;
    m_PlayerState[x].money = 1234567890;
    m_PlayerState[x].name = new char[50];
    m_PlayerState[x].msl_cur_type = (c3DObjectMissile::MSLTYPE)x;
    sprintf (m_PlayerState[x].name,"%s%d","Player ",x);
    m_numplayers++;
  }
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
    m_statusbartex = new CTexture("resource\\statusbar\\statusbar.dds");
}

void cGameState::_Delete(void)
{
  SAFE_DELETE(m_statusbartex);
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