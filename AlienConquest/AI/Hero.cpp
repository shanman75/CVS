#include "StdAfx.h"
#include "hero.h"
#include "objmgr.h"

int CHero::m_graph_init = 0;
CTexture *CHero::m_regular[4];
CTexture *CHero::m_dying[1];

CHero::CHero(void)
{
  CObj();
  if (!m_graph_init++) _LoadGraphics();
  m_boundrectnum = 4;
  m_boundrects = new RECT [m_boundrectnum];
//  SetRect((LPRECT)&m_boundrects[0],11,4,41,61);
//  SetRect((LPRECT)&m_boundrects[1],19,61,44,93);
//  SetRect((LPRECT)&m_boundrects[2],44,48,64,69);

  SetRect((LPRECT)&m_boundrects[0],50,26,70,143);
  SetRect((LPRECT)&m_boundrects[1],80,36,108,271);
  SetRect((LPRECT)&m_boundrects[2],108,256,126,271);
  SetRect((LPRECT)&m_boundrects[3],108,100,164,134);

  m_state = GHOST;
  m_lifes = 3;
  m_max_x= 320;
  m_max_y= 320;
  SetAccel(0,0);
  SetSpeed(0,0);
  m_time.GetTime();
  m_type = HERO;
  m_stayonscr = TRUE;
  m_z = 2;

  // Init weapons
  for (int x = 0; x < m_numweapons; x++)  {
	  m_powerup_eq[x] = FALSE;
	  m_powerup_rt[x] = 500;     // Default rate of fire is 500 ms
  }
  m_powerup_eq[PREG] = TRUE;
  m_powerup_rt[PREG] = 25;

  m_powerup_eq[PMISSILE] = FALSE;
  m_powerup_rt[PMISSILE] = 800;

  m_powerup_eq[PREG2] = FALSE;
  m_powerup_rt[PREG2] = 500;

  m_powerup_eq[PSPLIT] = FALSE;
  m_powerup_rt[PSPLIT] = 600;

}

CHero::~CHero(void)
{
	if (!--m_graph_init) _UnloadGraphics();
	delete []m_boundrects;
//	~CObj();
}

void CHero::_LoadGraphics(void)
{
   RECT trect;
   SetRect(&trect,1,1,126,97);
   OutputDebugString("Loading Hero graphics\n");
   SetRect(&trect,1,1,200,290);
   for (int x=0; x < 4; x++) {
	   trect.left=x*201+1;
	   trect.right = trect.left +200;
	   m_regular[x]=new CTexture("resource/mainrocketsmap.bmp",0xFFFF00FF,&trect);
   }
//   m_dying[0] = new CTexture("resource/explosion.bmp",0xFFFF00FF,NULL);
}

void CHero::_UnloadGraphics(void)
{
	delete m_regular[0];
	delete m_dying[0];
}

void CHero::paint(void)
{
	static BOOL paint = TRUE;
	switch (m_state) {
		case DYING:
			m_curtexture = m_regular[0];
			if (m_ani_tim.PeekTime() > 600) {			  
    		  if (--m_lifes < 0) m_state = DEAD;
			  else { m_state=GHOST; m_ani_tim.Reset(); m_dying_tim.Reset(); }
			}
			break;
		case DEAD:
			m_state = REGULAR;
			m_curtexture = m_regular[0];
			break;
		case FIRING:
			m_state = REGULAR;
			m_curtexture = m_regular[0];
			break;
		case GHOST:
			m_curtexture = m_regular[0];
			if (m_ani_tim.CmpTime(300)) paint = !paint ;
			if (m_dying_tim.CmpTime(3500)) { 
			  paint = TRUE; m_state = REGULAR; m_type=HERO;
			}
			break;
		case REGULAR:
		default:
			m_curtexture = m_regular[m_reg_seq%4];
			if (m_ani_tim.CmpTime(75)) m_reg_seq++;
			break;
	};
	if (paint)
	CObj::paint();
}

void CHero::move(void)
{
	if (m_mov_x.PeekTime() > 400) { m_speed_x=0; }
	if (m_mov_y.PeekTime() > 400) { m_speed_y=0; }
	CObj::move();
	return;

}

void CHero::FireWeapon(POWERUP weap)
{
	CObj *wp = NULL;

	switch(weap)
	{
	case PREG:
		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+175,m_dpos_y+127);
		wp->SetSpeed(370+m_speed_x,0);
		wp->SetAccel(-15,0);
		g_ObjMgr->add(wp);

		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+95,m_dpos_y+172);
		wp->SetSpeed(570+m_speed_x,m_speed_y);
		wp->SetAccel(-15,0);


		break;
	case PREG2:
		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+120,m_dpos_y+189);
		wp->SetSpeed(50+m_speed_x,0);
		wp->SetAccel(400,0);
		break;
	case PMISSILE:
		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+74,m_dpos_y+28);
		wp->SetSpeed(50+m_speed_x,0);
		wp->SetAccel(400,0);
		break;
	case PSPLIT:
		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+175,m_dpos_y+127);
		wp->SetSpeed(370+m_speed_x,270+m_speed_y);
		wp->SetAccel(-15,-15);
		g_ObjMgr->add(wp);
		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+175,m_dpos_y+127);
		wp->SetSpeed(370+m_speed_x,-270+m_speed_y);
		wp->SetAccel(-15,15);
		break;
	default:
		wp = NULL;
		break;
	}
	if (wp != NULL) g_ObjMgr->add(wp);
}

void CHero::Fire(void)
{
	for(int x = 0; x < m_numweapons; x++)
		if (m_powerup_eq[x] && m_powerup_tim[x].CmpTime(m_powerup_rt[x]))
			FireWeapon((POWERUP)x);
}

void CHero::event(EVENT event)
{
	if (m_state == DYING || m_state == DEAD) return;
	float spdx = 180.5;
	float spdy = 180.5;
	switch(event) {
		case UP:
			m_speed_y = -spdy; m_mov_y.Reset();
			break;
		case DOWN:
			m_speed_y = spdy; m_mov_y.Reset();
			break;
		case LEFT:
			m_speed_x = -spdx; m_mov_x.Reset();
			break;
		case RIGHT:
			m_speed_x = spdx; m_mov_x.Reset();
			break;
		case FIRE:
			if (m_state != FIRING) Fire(); 
			break;
		default:
			break;
	}
}

void CHero::Collision (CObj *colwith)
{
	m_ani_tim.Reset();
	m_speed_x=0;
	m_speed_y=0;
    m_state = DYING;
	m_type = HEROINVINCIBLE;
}