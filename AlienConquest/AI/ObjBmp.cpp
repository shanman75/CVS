#include "StdAfx.h"
#include "objbmp.h"

CObjBmp::CObjBmp(char *filename, int colkey)
{
	m_curtexture = new CTexture(filename,colkey);
	m_done = FALSE;
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
  switch (evnt)
  {
  case FIRE:
	  m_done = TRUE;
	  break;
  default:
	  break;
  }
}