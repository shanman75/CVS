#include "StdAfx.h"
#include "objmgr.h"
#include <stdio.h>

extern CHero *g_hero;
extern CHero2 *g_hero2;
extern int g_numenemies;
extern int g_levelnum;

CObjMgr *g_ObjMgr;
const int CObjMgr::m_numz=3;
char CObjMgr::m_scoreline[500];

CObjMgr::CObjMgr(void)
{
	m_numobj=0;

	// Load scoreboard textures
	RECT tool;
	SetRect(&tool,1,1,267,45);
	m_scoreboard = new CTexture("resource/toolbar/toolbarmain.PNG",0xFFFF00FF,&tool);
	m_scoremiddle = new CTexture("resource/toolbar/toolbarmiddle.PNG",0xFFFF00FF);
	SetRect(&tool,185,28,210,53);
	m_powerups[0] = new CTexture("resource/Hero/Heromap1.png",0xFFFF00FF,&tool);
	SetRect(&tool,185,54,210,79);
	m_powerups[1] = new CTexture("resource/Hero/Heromap1.png",0xFFFF00FF,&tool);
	SetRect(&tool,185,80,210,105);
	m_powerups[2] = new CTexture("resource/Hero/Heromap1.png",0xFFFF00FF,&tool);
	SetRect(&tool,237,28,262,53);
	m_powerups[3] = new CTexture("resource/Hero/Heromap1.png",0xFFFF00FF,&tool);

	reset();
}

CObjMgr::~CObjMgr(void)
{
	reset();
	for (int x = 0; x < 4; x++)
		SafeDelete(m_powerups[x]);
	SafeDelete(m_scoreboard);
	SafeDelete(m_scoremiddle);
}

void CObjMgr::coldet(void)
{
	for (int x=0; x < m_numobj; x++)
		for (int y=x+1; y < m_numobj; y++)
			if (m_obj[x]->CollisionDet(m_obj[y])) {
				m_obj[x]->Collision(m_obj[y]);
				m_obj[y]->Collision(m_obj[x]);
			};

	int objdel = 0;
	for (int x=0; x < m_numobj; x++) {
		if (m_obj[x] && m_obj[x]->m_state == CObj::DEAD) { 
//			OutputDebugString("Dead object needs deleting\n");
			delete m_obj[x];
			m_obj[x] = NULL;
			objdel++;
		}
	}
	// Now compact the list (if objects where deleted)
	int bobj = 0;
	if (objdel) {
		for (int x=0; x < m_numobj; x++)
		{
			if (m_obj[x])
				m_obj[bobj++] = m_obj[x];
		}
		m_numobj -= objdel;
	}
}

void CObjMgr::move()
{
	// Move the world
	m_world.move();

	for (int x=0; x < m_numobj; x++)
		m_obj[x]->move();
}

void CObjMgr::spawnOne(void)
{
	int objtype = rand() % 4;
	CObj *newobj;

	// Choose an object type
//	objtype=1;
	switch (objtype)
	{
	case 0:
		newobj = new CObjEnemy2;
		break;
	case 1:
		newobj = new CObjEnemy2;
		break;
	case 2:
	default:
		newobj = new CObjEnemy3;
		break;
	}
	newobj->SetPosition((float)(m_world.GetCurXEdge()),(float)(300+rand()%290-145));
	newobj->SetSpeed((float)(-150+rand()%50-25),(float)(rand()%50 - 25));
	newobj->SetAccel(0,0);
	add(newobj);
}

void CObjMgr::spawn(void)
{
	if (m_spawn_tim.CmpTime(m_spawn_interval)) {
		// Choose a spawn type..only one now
		int spn = rand()%(50+g_levelnum*6);
		if (spn < 41)
			spawnOne();
		else if (spn < 48)
		{
			spawnOne();
			spawnOne();
		}
		else {
			spawnOne(); spawnOne(); spawnOne();
		}
		// Choose next spawn interval

		switch (g_levelnum) {
			case 1:
				m_spawn_interval = 1800+rand()%2500;
				break;
			case 2:
				m_spawn_interval = 1300+rand()%1700;
				break;
			case 3:
				m_spawn_interval = 1000+rand()%1500;
				break;
			default:
				m_spawn_interval = 1000+rand()%1000;
				break;
		}
	}
}

void CObjMgr::paint()
{
	static char livestr[500];
	for (int z=m_numz; z >= 0; z--) 
	  for (int x=0; x < m_numobj; x++)
		if (m_obj[x] != NULL && m_obj[x]->m_z == z) m_obj[x]->paint();	

	// Paint DashBoard
	//sprintf(m_scoreline,"%i",m_player1_score);
	//g_D3DObject->DrawTextStr(5,5,0xFFFF00FF,m_scoreline);

	PaintDashBoards();
}

void CObjMgr::add(CObj *add)
{
	if (add!=NULL) m_obj[m_numobj++] = add;
}

void CObjMgr::del(CObj *del)
{
//	OutputDebugString("Deleting object\n");
	if (del != NULL) {
		int found= 0;
		// Find the pointer
		for (int x = 0; x < m_numobj; x++)
		{
			if (m_obj[x] == del) { found = 1; delete m_obj[x]; }
			if (found) { m_obj[x]= m_obj[x+1]; }
		}
		if (found)
		 m_obj[m_numobj--] = NULL;
		else
		 OutputDebugString("Object NOT found\n");
	}
	OutputDebugString("Deleted object\n");
}

void CObjMgr::Score(int score, int playernum)
{
	if (playernum == 1) { m_player1_score += score; }
}

void CObjMgr::reset(void)
{
	if (m_numobj > 0)
	for (int x = 0; x < m_numobj ; x++)
		SafeDelete(m_obj[x]);
	m_numobj = 0;
	m_obj[0] = NULL;	
	m_spawn_interval = 1200;
	m_player1_score = 0;
	m_player1_scorexy.x =150;
	m_player1_scorexy.y = 5;
	m_player1_livesxy.x = 520;
	m_player1_livesxy.y = 5;
	m_spawn_tim.Reset();

	m_world.reset();

}

void CObjMgr::DrawText(const char *textstr, int x, int y)
{
	m_D3DText.DrawString(textstr,x,y);
}

void CObjMgr::PaintDashBoards()
{
	static char livestr[500];
	int base = 10;
	
	m_scoreboard->Paint(base,1);
	if(CHero::GetLives() == 0) {
		m_D3DText.DrawString("PLAYER 1",base+10,15);
	}
	else {
	if (CHero::HasPowerup(CHero::PLASER1))   m_powerups[0]->Paint(base+214,13);
	if (CHero::HasPowerup(CHero::PSPLIT))    m_powerups[1]->Paint(base+214,35);
	if (CHero::HasPowerup(CHero::PMISSILE))  m_powerups[2]->Paint(base+236,13);
	if (CHero::HasPowerup(CHero::PMISSILE2)) m_powerups[3]->Paint(base+236,35);
	//m_D3DText.DrawScore(g_hero->GetLives(),m_player1_scorexy);
	//sprintf(livestr,"LIVES %i",g_Hero->GetLives());
	base = base + 5*30+3+2*6;
	m_D3DText.DrawScore(CHero::GetScore(),base,15);
	base += 1*30+15;
	m_D3DText.DrawScore(CHero::GetLives(),base,15);
	//m_D3DText.DrawScore(999999,180+5+2*6,15);
	}

	// Player2 Board
	base = 520;
	m_scoreboard->Paint(base,1);
	if(CHero2::GetLives() == 0) {
		m_D3DText.DrawString("PLAYER 2",base+10,15);
	}
	else {
	if (CHero2::HasPowerup(CHero2::PLASER1)) m_powerups[0]->Paint(base+214,13);
	if (CHero2::HasPowerup(CHero2::PSPLIT)) m_powerups[1]->Paint(base+214,35);
	if (CHero2::HasPowerup(CHero2::PMISSILE))m_powerups[2]->Paint(base+236,13);
	if (CHero2::HasPowerup(CHero2::PMISSILE2))m_powerups[3]->Paint(base+236,35);
		//	m_D3DText.DrawScore(m_player1_score,m_player1_scorexy);
		//sprintf(livestr,"LIVES %i",g_Hero->GetLives());
		base = base + 5*30+3+2*6;
		m_D3DText.DrawScore(CHero2::GetScore(),base,15);
		base += 1*30+15;
		m_D3DText.DrawScore(CHero2::GetLives(),base,15);
	}
	m_scoremiddle->Paint(300,1);
	m_D3DText.DrawScore(g_numenemies,479,15);
	m_D3DText.DrawScore(g_levelnum,355,15);
}