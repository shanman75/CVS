#include "StdAfx.h"
#include "Hero.h"
#include "objmgr.h"
#include "sound.h"

extern CSoundManager *g_SndMgr;

int CHero::m_graph_init = 0;
CTexture *CHero::m_regular[4];
CTexture *CHero::m_dying[1];
CTexture *CHero::m_hit[2];
int CHero::m_score;
int CHero::m_lifes;
BOOL CHero::m_powerup_eq[m_numweapons];

RECT CHero::m_myboundrects[4];

CHero::CHero(void)
{
  CObj();
  if (!m_graph_init++) _LoadGraphics();
  m_boundrectnum = 1;
  m_boundrects = m_myboundrects;

  m_reg_seq.SetParams(3,CAnimate::ROLL,200,0);
  m_hit_seq.SetParams(2,CAnimate::OSCILLATE,30,120);

  m_state = GHOST;

  m_max_x= 320;
  m_max_y= 320;
  m_dmg = 0;
  SetAccel(0,0);
  SetSpeed(CWorld::GetCurXSpeed(),CWorld::GetCurYSpeed());
 
  m_time.GetTime();
  //m_type = HERODEAD;
  m_stayonscr = TRUE;
  m_z = 1;

  // Init weapons
  m_powerup_eq[PREG] = TRUE;
  m_powerup_rt[PREG] = 260;

  m_powerup_rt[PMISSILE] = 1120;
  m_powerup_rt[PSPLIT] = 320;
  m_powerup_rt[PLASER1] = 500;
  m_powerup_rt[PMISSILE2] = 1200;

}

CHero::~CHero(void)
{
//	g_SndMgr->play(ENEMY3_DEAD,0);
	if (!--m_graph_init) _UnloadGraphics();
}

void CHero::InitCharacter()
{
  m_score = 0;
  m_lifes = 0;
  for (int x = 0; x < m_numweapons; x++)  {
	  m_powerup_eq[x] = FALSE;
//	  m_powerup_rt[x] = 500;     // Default rate of fire is 500 ms
  }
  m_state = GHOST;
  m_type = HERODEAD;
}

void CHero::Live()
{
	m_state = GHOST;
	m_type = HEROINVINCIBLE;
    m_powerup_eq[PREG] = TRUE;
	m_lifes=3;
}

void CHero::_LoadGraphics(void)
{
   char *tfile = "resource/Hero/Heromap1.png";
   char *tfile2 = "resource/Hero/herohit.png";
   RECT trect;
   SetRect(&trect,1,1,126,97);
   OutputDebugString("Loading Hero graphics\n");
   SetRect(&trect,1,124,87,246);
   for (int x=0; x < 4; x++) {
	   trect.left=x*87+1;
	   trect.right = trect.left +86;
	   m_regular[x]=new CTexture(tfile,0xFFFF00FF,&trect);
   }
   SetRect(&trect,1,1,87,123);
   for (int x=0; x < 2; x++) {
	   trect.left=x*87+1;
	   trect.right = trect.left +86;
	   m_hit[x]=new CTexture(tfile2,0xFFFF00FF,&trect);
   }


//   m_dying[0] = new CTexture("resource/explosion.bmp",0xFFFF00FF,NULL);

   // Set Bounding rects
  SetRect((LPRECT)&m_myboundrects[0],21,11,73,112);
//  SetRect((LPRECT)&m_myboundrects[1],41,25,49,42);
//  SetRect((LPRECT)&m_myboundrects[2],34,72,53,124);
//  SetRect((LPRECT)&m_myboundrects[3],20,42,69,72);

  // BIG ONE for testing...
  //SetRect((LPRECT)&m_myboundrects[0],9,5,82,119);

}

void CHero::_UnloadGraphics(void)
{
   for (int x=0; x < 4; x++)
	delete m_regular[x];
   for (int x=0; x < 2; x++)
	delete m_hit[x];
//	delete m_dying[0];
}

void CHero::paint(void)
{
//Fire();
	if (m_lifes<=0) {m_type = HERODEAD; return; }
	static BOOL paint = TRUE;
	switch (m_state) {
		case HIT:
			m_curtexture = m_hit[m_hit_seq.GetCurFrame()];
			if (m_hit_seq.Done()) { m_state = REGULAR; }
			break;
		case DYING:
			m_curtexture = m_regular[m_reg_seq.GetCurFrame()];
			if (m_ani_tim.PeekTime() > 600) {			  
    		  if (--m_lifes < 0) m_state = DEAD;
			  else { m_state=GHOST; m_ani_tim.Reset(); m_dying_tim.Reset(); }
			}
			break;
		case DEAD:
			m_state = REGULAR;
			m_curtexture = m_regular[m_reg_seq.GetCurFrame()];
			break;
		case FIRING:
			m_state = REGULAR;
			m_curtexture = m_regular[m_reg_seq.GetCurFrame()];
			break;
		case GHOST:
			m_dmg = 0;
			m_curtexture = m_regular[m_reg_seq.GetCurFrame()];
			if (m_ani_tim.CmpTime(300)) paint = !paint;
			if (m_dying_tim.CmpTime(3500)) { 
			  paint = TRUE; 
			  m_state = REGULAR; 
			  m_type=HERO;
			}
			break;
		case REGULAR:
		default:
			m_curtexture = m_regular[m_reg_seq.GetCurFrame()];
			break;
	};
	if (paint)
	CObj::paint();
}

void CHero::move(void)
{
	if (m_mov_x.PeekTime() > 300) { m_speed_x=CWorld::GetCurXSpeed(); }
	if (m_mov_y.PeekTime() > 300) { m_speed_y=CWorld::GetCurYSpeed(); }
	CObj::move();
	return;
}

void CHero::FireWeapon(POWERUP weap)
{
	CObj *wp = NULL;
	CObjHeroWeaponMain *wm = NULL;
	CObjHeroWeaponMissile *wmm = NULL;
	CObjHeroWeaponLaser *wl = NULL;

	switch(weap)
	{
	case PREG:
		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+80,m_dpos_y+54);
		wp->SetSpeed(380+m_speed_x,0);
		wp->SetAccel(-15,0);
		wm = (CObjHeroWeaponMain *)wp; wm->SetHero(this); wm = NULL;
		g_SndMgr->play(HERO_FIREREG);
		break;
	case PMISSILE:
		// Need to support weapon tagging
		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+82,m_dpos_y+53);
		wp->SetSpeed(110+m_speed_x,m_speed_y+320);
		wp->SetAccel(200,-200);
		wmm = (CObjHeroWeaponMissile *)wp; wmm->SetHero(this); wmm = NULL;
		g_ObjMgr->add(wp);

		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+84,m_dpos_y+56);
		wp->SetSpeed(110+m_speed_x,m_speed_y-320);
		wp->SetAccel(200,200);
		wmm = (CObjHeroWeaponMissile *)wp; wmm->SetHero(this); wmm = NULL;
		g_SndMgr->play(HERO_FIREMIS);
		break;
	case PMISSILE2:
		// Need to support weapon tagging
		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+59,m_dpos_y+83);
		wp->SetSpeed(170+m_speed_x,m_speed_y+50);
		wp->SetAccel(230,-230);
		wmm = (CObjHeroWeaponMissile *)wp; wmm->SetHero(this); wmm = NULL;
		g_ObjMgr->add(wp);

		wp = new CObjHeroWeaponMissile;
		wp->SetPosition(m_dpos_x+38,m_dpos_y+10);
		wp->SetSpeed(170+m_speed_x,m_speed_y-50);
		wp->SetAccel(230,230);
		wmm = (CObjHeroWeaponMissile *)wp; wmm->SetHero(this); wmm = NULL;
		break;

	case PSPLIT:
		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+73,m_dpos_y+42);
		wp->SetSpeed(380+m_speed_x,-190+m_speed_y);
		wp->SetAccel(-15,50);
		wm = (CObjHeroWeaponMain *)wp; wm->SetHero(this); wm = NULL;
		g_ObjMgr->add(wp);
		wp = NULL;

		wp = new CObjHeroWeaponMain;
		wp->SetPosition(m_dpos_x+74,m_dpos_y+53);
		wp->SetSpeed(380+m_speed_x,190+m_speed_y);
		wp->SetAccel(-15,-50);
		wm = (CObjHeroWeaponMain *)wp; wm->SetHero(this); wm = NULL;
		g_ObjMgr->add(wp);
		wp = NULL;
		break;
	case PLASER1:
		wp = new CObjHeroWeaponLaser;
		wp->SetPosition(m_dpos_x+78,m_dpos_y+44);
		wp->SetSpeed(900+m_speed_x,m_speed_y);
		wp->SetAccel(-15,0);
		wl = (CObjHeroWeaponLaser *)wp; wl->SetHero(this); wl = NULL;
		g_ObjMgr->add(wp);
		wp = NULL;
		g_SndMgr->play(HERO_FIRELAS);
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
	if (m_ani_tim.Paused()==TRUE) return;
	if (m_type == HERODEAD) {
		if (event == FIRE) {
//			m_score=0;
			m_lifes=3;
			SetSpeed(CWorld::GetCurXSpeed(),CWorld::GetCurYSpeed());
			SetAccel(0,0);
			SetPosition(CWorld::GetCurX()+30,CWorld::GetCurY()+150);
//			InitCharacter();
			m_type = HEROINVINCIBLE;
			m_state = GHOST;
			for (int x = 0; x < m_numweapons; x++)  {
				m_powerup_eq[x] = FALSE;
			//	  m_powerup_rt[x] = 500;     // Default rate of fire is 500 ms
			}
			m_powerup_eq[PREG] = TRUE;
		}
		return;
	}
	if (m_state == DYING || m_state == DEAD) return;
	float spdx = 260.5;
	float spdy = 180.5;
	switch(event) {
		case UP:
			m_speed_y = -spdy+CWorld::GetCurYSpeed(); m_mov_y.Reset();
			break;
		case DOWN:
			m_speed_y = spdy+CWorld::GetCurYSpeed(); m_mov_y.Reset();
			break;
		case LEFT:
			m_speed_x = -spdx*.80+CWorld::GetCurXSpeed(); m_mov_x.Reset();
			break;
		case RIGHT:
			m_speed_x = spdx+CWorld::GetCurXSpeed(); m_mov_x.Reset();
			break;
		case FIRE:
			if (m_state != FIRING) Fire(); 
			break;
		case GOD:
			for (int x=0; x < m_numweapons; x++)
				m_powerup_eq[x] = TRUE;
			break;
		default:
			break;
	}
}

void CHero::Collision (CObj *colwith)
{
	if (colwith->GetType() == O_POWERUP) {
		CObjPowerUp *p = (CObjPowerUp *) colwith;
		switch (p->GetPowerUp()){
			case 0:   // LASER
				m_powerup_eq[PLASER1] = TRUE;break;
			case 1:   // PMS2
				m_powerup_eq[PMISSILE2] = TRUE;break;
			case 2:   // PSPL
				m_powerup_eq[PSPLIT] = TRUE;break;
			case 3:   // PLSR1
				m_powerup_eq[PMISSILE] = TRUE;break;
			default:
				break;
		}
		g_SndMgr->play(MENU_SELECT);
//		m_powerup_eq[p->GetPowerUp()] = TRUE;
	}
	else {
		if (m_dmg++ > 5) {
			m_ani_tim.Reset();
			m_speed_x=CWorld::GetCurXSpeed();
			m_speed_y=CWorld::GetCurYSpeed();
			m_state = DYING;
			m_type = HEROINVINCIBLE;
			g_SndMgr->play(HERO_DEAD);
		}
		else {
			m_state = HIT;
			m_hit_seq.Reset();
			g_SndMgr->play(HERO_HIT);
		}
	}
}

void CHero::Score(int sc)
 { m_score+=sc; 
 }