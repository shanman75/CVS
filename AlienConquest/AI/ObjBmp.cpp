#include "StdAfx.h"
#include "objbmp.h"

CObjBmp::CObjBmp(char *filename, int colkey, BOOL anyenabled)
{
	//m_mytexture = new CTexture(filename,colkey);
	m_mytexture = new CTexture(filename,colkey,NULL,1024,1024);
	m_done = FALSE;
	m_dpos_x = 0;
	m_dpos_y = 0;
	m_curtexture = m_mytexture;
	m_anyenabled = anyenabled;
//	m_world.reset();
}

CObjBmp::~CObjBmp(void)
{
	delete m_curtexture;
}

void CObjBmp::move (void)
{
}

void CObjBmp::event(EVENT evnt)
{
 // BOOL savev = m_done;
  switch (evnt)
  {
  case FIRE:
	  m_done = TRUE;
	  break;
  case ANY:
	  if (m_anyenabled) m_done = TRUE;
	  break;
  default:
	  break;
  }
  //if (savev != m_done) g_SndMgr->play(MENU_SELECT);
}