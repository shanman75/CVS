#include "StdAfx.h"
#include "obj.h"
#include "math.h"
#include "objmgr.h"

const float CObj::SSCALE=(float)0.003;
const float CObj::ASCALE=(float)0.0014;

CObj::CObj(void)
{
	m_max_x=(float)50.2;
	m_max_y=(float)45.5;
	m_time.GetTime();
	m_age_timer.GetTime();
	m_z = 1;
	m_age = 0;
	m_curtexture = NULL;
	m_boundrectnum = 0;
	m_state = REGULAR;
	m_type = NOTHING;
	m_stayonscr = FALSE;
	m_playernum = 1;
	m_done = FALSE;
}

CObj::~CObj(void)
{
}

void CObj::paint(void)
{
	D3DXVECTOR2 pnt;
	D3DXVECTOR2 tex;

	pnt.x = m_dpos_x;
	pnt.y = m_dpos_y;

	tex.x = (float)m_curtexture->GetWidth();
	tex.y = (float)m_curtexture->GetHeight();

	if (m_world.ToScreen(&pnt,&tex)) {
		//OutputDebugString("Object is on the screen\n");
		if (m_curtexture)
			m_curtexture->Paint(&pnt);
	}
	else if (m_world.IsWayLeft(&pnt,&tex)) {
		OutputDebugString("Object is off the screen, AND way left\n");
		this->m_state = DEAD;
	}
	else { OutputDebugString("Object is OFF THE SCREEN BUT NOT WAY LEFT\n"); }
}

void CObj::move(void)
{
	if (m_age  && ((int)m_age_timer.PeekTime() > m_age)) {m_state = DEAD; return;}
	float tm = (float)m_time.GetTime()/1000;

	// char buff[500];
	// sprintf(buff,"Moving tm=%f\n",tm);
	// OutputDebugString(buff);
	// sprintf(buff,"Old speed is x,y=%.2f,%.2f -  %.4f %.4f\n",m_speed_x,m_speed_y,
	// m_accel_x,m_accel_y);
	// OutputDebugString(buff);

	// Calculate Distance
	float dlt_x = (float)(m_speed_x*tm + 0.5*m_accel_x*tm*tm);
	float dlt_y = (float)(m_speed_y*tm + 0.5*m_accel_y*tm*tm);
	// sprintf(buff,"Delta x,y=%.2f,%.2f - %.2f, %.2f\n",dlt_x,dlt_y,
	// m_dpos_x, m_dpos_y
	// OutputDebugString(buff);

	// Take into account acceleration
	m_dpos_x += dlt_x;
	m_dpos_y += dlt_y;

/*
// SHould be taken care of by World code now...
    float MARGIN_X = 50;
	float MARGIN_Y = 50;
	if (m_dpos_x < (0-MARGIN_X)) m_dpos_x = WIDTH;
	if (m_dpos_y < (0-MARGIN_Y)) m_dpos_y = HEIGHT;
	if (m_dpos_x > WIDTH) m_dpos_x = 0-MARGIN_X;
	if (m_dpos_y > HEIGHT) m_dpos_y = 0-MARGIN_Y;
	//m_pos_x = (int) m_dpos_x % WIDTH;
	//m_pos_y = (int) m_dpos_y % HEIGHT;
	//sprintf(buff,"X,Y = %i,%i\n",m_pos_x, m_pos_y);
	//OutputDebugString(buff);
*/

	// New speed
	m_speed_x += m_accel_x*tm;
	m_speed_y += m_accel_y*tm;

	if (fabs(m_speed_x) > m_max_x) m_accel_x=0;
	if (fabs(m_speed_y) > m_max_y) m_accel_y=0;

	if (m_stayonscr) {
		// Keep object on the screen
		if (m_curtexture)
		m_dpos_x = m_world.CullX(m_dpos_x,(float)m_curtexture->GetWidth());
	}

	if (m_curtexture)
		m_dpos_y = m_world.CullY(m_dpos_y,(float)m_curtexture->GetHeight());
}

void CObj::SetSpeed(float xspeed,float yspeed)
{
	m_speed_x = xspeed;
	m_speed_y = yspeed;
}

void CObj::SetAccel(float xaccel,float yaccel)
{
	m_accel_y = yaccel;
	m_accel_x = xaccel;
}

void CObj::SetPosition(float x, float y)
{
	m_dpos_x = x;
	m_dpos_y = y;
}

void CObj::accel(float xdelta, float ydelta)
{
  m_accel_x+=xdelta;
  m_accel_y+=ydelta;
}

LPRECT CObj::m_tr_x;
LPRECT CObj::m_tr_y;

BOOL CObj::TestRect(const RECT *cmp1, const POINT ptx, const RECT *cmp2, const POINT pty)
{
  if (!m_tr_x) m_tr_x = new RECT;
  if (!m_tr_y) m_tr_y = new RECT;
  
  CopyRect(m_tr_x,cmp1);
  CopyRect(m_tr_y,cmp2);
  OffsetRect(m_tr_x,ptx.x,ptx.y);
  OffsetRect(m_tr_y,pty.x,pty.y);

  if (m_tr_x->left > m_tr_y->right || m_tr_y->left > m_tr_x->right ||
	  m_tr_x->top > m_tr_y->bottom || m_tr_y->top > m_tr_x->bottom )
	  return FALSE;
  else
	  return TRUE;
}

BOOL CObj::CollisionDet(CObj *colobj)
{
  if (!CanCollide(this->m_type,colobj->m_type)) return FALSE;

  POINT ptx = {(LONG)this->m_dpos_x, (LONG) this->m_dpos_y};
  POINT pty = {(LONG)colobj->m_dpos_x, (LONG) colobj->m_dpos_y};

  for (int x=0; x< this->m_boundrectnum; x++)
	  for (int y=0; y< colobj->m_boundrectnum; y++)
		if (TestRect(&this->m_boundrects[x],ptx,&colobj->m_boundrects[y],pty))
			return TRUE;
  return FALSE;
}

void CObj::Collision(CObj *colwith)
{
	OutputDebugString("COLLISION!\n");
	m_state = DEAD;
}

BOOL CObj::CanCollide(OTYPE x, OTYPE y)
{
  if ( (x == ENEMY && y == HEROWEAPON )
	   || (x == HEROWEAPON && y == ENEMY) ) return TRUE;
  else if ( (x == HERO && y == ENEMYWEAPON) 
	  || ( x == ENEMYWEAPON && y == HERO) ) return TRUE;
  else if ( (x == HERO && y == ENEMY) 
	  || (y == HERO && x == ENEMY)) return TRUE;
  else if ( (x == HERO && y == O_POWERUP)
	  || (y== O_POWERUP && x == HERO) ) return TRUE;
  return FALSE;
}

BOOL CObj::done(void)
{
	return m_done;
}