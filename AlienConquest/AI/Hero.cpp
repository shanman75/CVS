#include "StdAfx.h"
#include "hero.h"
#include "objmgr.h"

int CHero::m_graph_init = 0;
CTexture *CHero::m_regular[1];
CTexture *CHero::m_dying[1];

CHero::CHero(void)
{
  CObj();
  if (!m_graph_init++) _LoadGraphics();
  m_boundrectnum = 3;
  m_boundrects = new RECT [m_boundrectnum];
  SetRect((LPRECT)&m_boundrects[0],11,4,41,61);
  SetRect((LPRECT)&m_boundrects[1],19,61,44,93);
  SetRect((LPRECT)&m_boundrects[2],44,48,64,69);
  m_state = REGULAR;
  m_max_x= 320;
  m_max_y= 320;
  SetAccel(0,0);
  SetSpeed(0,0);
  m_time.GetTime();
  m_type = HERO;
  m_stayonscr = TRUE;

  // Init weapons
  for (int x = 0; x < m_numweapons; x++)  {
	  m_powerup_eq[x] = FALSE;
	  m_powerup_rt[x] = 500;     // Default rate of fire is 500 ms
  }
  m_powerup_eq[0] = TRUE;
  m_powerup_rt[0] = 1;

  m_powerup_eq[PMISSILE] = TRUE;
  m_powerup_rt[PMISSILE] = 8000;

  m_powerup_eq[PREG2] = TRUE;
  m_powerup_rt[PREG2] = 100;

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
   m_regular[0] = new CTexture("resource/herodumdum2.bmp",0xFFFF00FF,NULL);
   m_dying[0] = new CTexture("resource/explosion.bmp",0xFFFF00FF,NULL);
}

void CHero::_UnloadGraphics(void)
{
	delete m_regular[0];
	delete m_dying[0];
}

void CHero::paint(void)
{
	switch (m_state) {
		case DYING:
			m_curtexture = m_dying[0];
			if (m_ani_tim.PeekTime() > 600) { m_state=GHOST; }
			break;
		case DEAD:
			m_curtexture = m_regular[0];
			break;
		case FIRING:
			m_state = REGULAR;
			m_curtexture = m_regular[0];
			break;
		case REGULAR:
		default:
			if (m_regular[0] != NULL)
				m_curtexture = m_regular[0];
			break;
	};
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
		wp->SetPosition(m_dpos_x+50,m_dpos_y+52);
		wp->SetSpeed(350+m_speed_x,0);
		wp->SetAccel(-15,0);
		break;
	case PREG2:
		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+29,m_dpos_y+90);
		wp->SetSpeed(350+m_speed_x,0);
		wp->SetAccel(-15,0);
	case PMISSILE:
		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+35,m_dpos_y+5);
		wp->SetSpeed(50+m_speed_x,0);
		wp->SetAccel(400,0);
	default:
		break;
	}
	if (wp) g_ObjMgr->add(wp);
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
			if (m_state != FIRING && m_fir_tim.PeekTime() > 500 ) { m_fir_tim.Reset(); Fire(); }
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