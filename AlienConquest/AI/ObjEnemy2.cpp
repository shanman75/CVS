#include "StdAfx.h"
#include "objenemy2.h"
#include "objenemyweapon2.h"
#include "objmgr.h"
#include "Sound.h"

extern CSoundManager *g_SndMgr;
extern int g_levelnum;

int CObjEnemy2::m_graph_init = 0;
extern int g_numenemies;

CTexture **CObjEnemy2::m_regular = new CTexture*[3];
CTexture **CObjEnemy2::m_firing = new CTexture*[9];
CTexture **CObjEnemy2::m_jetting = new CTexture*[12];
CTexture **CObjEnemy2::m_hit = new CTexture*[2];
RECT CObjEnemy2::m_myboundrects[4];
const char *CObjEnemy2::m_grdir = "enemies/enemy2";

void CObjEnemy2::move(void)
{
	float cur_spd_x;
	if (m_state == JETTING) { cur_spd_x = m_speed_x; m_speed_x = -250; CObj::move(); m_speed_x=cur_spd_x;  }
	else CObj::move();
}

CObjEnemy2::CObjEnemy2(void)
{
	CObj();

	if (!m_graph_init++) _LoadGraphics();
	m_state = REGULAR;
	m_max_x= m_org_max_x = 120;
	m_max_y=100;
	m_type = ENEMY;
	m_time.Reset();
	m_curdamage = 0;

	m_jet_time.setInterval(400+rand()%2100);
	m_fire_time.setInterval(100+rand()%900);

	m_age_time.setInterval(9000);

	m_boundrectnum = 4;
	m_boundrects = m_myboundrects;

	m_jet_seq.SetParams(4,CAnimate::OSCILLATE,200,1400,0);
	m_fir_seq.SetParams(3,CAnimate::ROLL,190,0,0);
	m_hit_seq.SetParams(2,CAnimate::ROLL,30,120,0);

	m_behave_tim.setInterval(500+rand()%1000);
	m_behaviour = RANDOM;
}

CObjEnemy2::~CObjEnemy2(void)
{
	if(!--m_graph_init)_UnloadGraphics();
	g_numenemies--;
}

void CObjEnemy2::paint()
{
	D3DXVECTOR2 pnt;
	pnt.x = m_dpos_x;
	pnt.y = m_dpos_y;

	_Behaviour();

	if (m_age_time.CmpTime()) m_behaviour = EXIT_STAGE_LEFT;
    if (m_state == REGULAR && m_fire_time.CmpTime()) Fire();
	if (m_state == REGULAR && m_jet_time.CmpTime()) Jet();

	switch (m_state) {
		case FIRING:
			m_curtexture = m_firing[(m_curdamage*3)+m_fir_seq.GetCurFrame()];
			if (m_fir_seq.Done()) m_state = REGULAR;
		    break;
		case JETTING:
			m_curtexture = m_jetting[(m_curdamage*4)+m_jet_seq.GetCurFrame()];
			if (m_jet_seq.Done()) { m_state = REGULAR; }
			break;
		case DEAD:
			m_curtexture = NULL;
			break;
		case HIT:
			m_curtexture = m_hit[m_hit_seq.GetCurFrame()];
			if (m_hit_seq.Done()) { m_state = REGULAR; }
			break;
		case REGULAR:
		default:
			m_curtexture = m_regular[m_curdamage];
			break;
	};
	if (m_state != DEAD)
	CObj::paint();
}

void CObjEnemy2::Jet()
{
	if (m_state == REGULAR) {
		m_state = JETTING;
		m_jet_seq.Reset();
		m_jet_nxt = 1400+rand()%500;
	}
}
void CObjEnemy2::Fire()
{
	if (m_state == REGULAR) {
		m_state = FIRING;
		m_fir_seq.Reset();
		m_fir_nxt = 400+rand()%500;

		CObjEnemyWeapon2 *bull = new CObjEnemyWeapon2;
		bull->SetPosition(m_dpos_x+8,m_dpos_y+87);
		bull->SetSpeed(-750,0);
		bull->SetAccel(-50,0);
		g_ObjMgr->add(bull);

		g_SndMgr->play(ENEMY2_FIRE);
	}
}

void CObjEnemy2::_LoadGraphics()
{
   OutputDebugString("Loading Cenemy2 graphics\n");
   char tpath[500];
   RECT trect;
   int dmg=0;
   for (int d=0; d < m_numdamage; d++) {
		SetRect(&trect,1,1,126,97);
   		sprintf (tpath,"resource/%s/headdmg%i.png",m_grdir,d);
   		m_regular[d] = new CTexture(tpath,0xFFFF00FF,&trect);

		SetRect(&trect,1,1,126,97);
		for (int x=0; x < 3; x++) {
			int offset = d*3;
			sprintf (tpath,"resource/%s/headfiredmg%i.png",m_grdir,d);
			trect.left=x*126+1;
			trect.right = trect.left +125;
			m_firing[offset+x]=new CTexture(tpath,0xFFFF00FF,&trect);
		}
		SetRect(&trect,1,1,126,97);
		for (int x=0; x < 4; x++) {
			int offset = d*4;
			sprintf (tpath,"resource/%s/headjetdmg%i.png",m_grdir,d);
			trect.left=x*126+1;
			trect.right = trect.left +125;
			m_jetting[offset+x]=new CTexture(tpath,0xFFFF00FF,&trect);
		}
   }
   SetRect(&trect,1,1,126,97);
   for (int x=0; x < 2; x++) {
		sprintf (tpath,"resource/%s/headhit.png",m_grdir);
		trect.left=x*126+1;
		trect.right = trect.left +125;
		m_hit[x] = new CTexture(tpath,0xFFFF00FF,&trect);
   }

   	SetRect((LPRECT)&m_myboundrects[0],35,7,72,69);
	SetRect((LPRECT)&m_myboundrects[1],72,27,99,43);
	SetRect((LPRECT)&m_myboundrects[2],55,69,61,84);
	SetRect((LPRECT)&m_myboundrects[3],8,86,47,91);
}

void CObjEnemy2::Collision(CObj *colwith)
{
	if (++m_curdamage >= m_numdamage) {
	   m_state=DEAD;
	   g_SndMgr->play(ENEMY2_DEAD);
	   CObj::Collision(colwith);
	}
	else {
		m_state = HIT;
		g_SndMgr->play(ENEMY2_HIT);
		m_hit_seq.Reset();
	}
}
void CObjEnemy2::_UnloadGraphics()
{
	for (int x=0; x < 3; x++)
	  delete m_regular[x];
	for (int x=0; x < 9; x++)
	  delete m_firing[x];
	for (int x=0; x < 12; x++)
	  delete m_jetting[x];
	for (int x=0; x < 2; x++)
	  delete m_hit[x];
}

void CObjEnemy2::_Behaviour(void)
{
   if (m_behaviour != EXIT_STAGE_LEFT && m_behave_tim.CmpTime()) { 
	   int nxtchoise = (rand()%1000);
	   if (nxtchoise < 300) {
		   m_behave_tim.setInterval(1200);
		   m_behaviour = STAYANDFIRE;
	   }
	   else if (nxtchoise < 600) {
		   m_behaviour = RANDOM;
		   m_behave_tim.setInterval(8000);
	   }
	   else if (nxtchoise < 1000) {
		   m_behaviour = EVADE;
		   m_behave_tim.setInterval(5000);
		   m_speed_x = CWorld::GetCurXSpeed()-50;
		   if (m_dpos_y < 250) 
			   m_speed_y = -(m_dpos_y*m_dpos_y/150);
		   else
			   m_speed_y = (m_dpos_y-600)*(m_dpos_y-600)/150;
	   }
   }
   switch (m_behaviour)
   {
   case RANDOM:
	   if (m_rand_time.CmpTime()) {
		   m_rand_time.setInterval(100+rand()%400);
		   SetAccel(rand()%400-200,rand()%300-150);
		   m_jet_time.setInterval(400+rand()%1200);
		   m_fire_time.setInterval(100+rand()%600);
	   }
	   break;
   case EXIT_STAGE_LEFT:
	   m_speed_x = -350;
	   m_speed_y = 0;
	   break;
   case STAYANDFIRE:
	   m_jet_time.setInterval(60000);
	   m_speed_x = CWorld::GetCurXSpeed();
	   break;
   case EVADE:
	   m_fire_time.setInterval(60000);
	   m_jet_time.setInterval (60000);
	   if (m_dpos_y > 250)
		   SetAccel(0,-600);
	   else 
		   SetAccel(0,600);
	   break;
   default:
	   break;
   }
}