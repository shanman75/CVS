#include "StdAfx.h"
#include "text.h"
#include "d3dx8.h"

int CText::m_graph_init;

CText::CText(void)
{
	if (!m_graph_init++) {
		_InitGraphics();
	}
}

CText::~CText(void)
{
	if (!--m_graph_init) {
		_DeleteGraphics();
	}
}

void CText::DrawString(const char *str, int x, int y)
{
   D3DXVECTOR2 pnt;
   pnt.x = x;
   pnt.y = y;
   DrawString(str,pnt);
}

void CText::DrawScore (int num, D3DXVECTOR2 endpnt)
{
   endpnt.x -= 32;
   // Special case of 0
   if (num==0) m_chars[28]->Paint(&endpnt);
   // point is actually the END here...
   while (num > 0 && endpnt.x > 0) 
   {
	   m_chars[28 + num % 10]->Paint(&endpnt);
	   num = num / 10;
	   endpnt.x -= 32;
   }
}

void CText::DrawString(const char *str, D3DXVECTOR2 pnt)
{
  char *tptr = (char *) str;
  char ptr;
  int pos;
  
  ptr = tptr[0];

  while ((ptr = tptr[0]) != '\0')
  {
	  // Lookup character
	  if ( (ptr >= 'a') && (ptr <= 'z'))
		  pos = ptr-'a';
	  else if ( (ptr >= 'A') && (ptr <= 'Z'))
		  pos = ptr - 'A';
	  else if ( (ptr >= '0') && (ptr <= '9'))
		  pos = ptr - '0' + 28;
	  else if ( ptr == '.') 
		  pos = 27;
	  else if ( ptr == ' ')
		  pos = 26;

	  m_chars[pos]->Paint(&pnt);
	  pnt.x+=32;
	  tptr++;
  }
}

void CText::Testdraw(void)
{
  D3DXVECTOR2 pnt (50,50);
  m_chars[0]->Paint(&pnt);
}

void CText::_InitGraphics()
{
   RECT rct;

   // Load the letters
   SetRect(&rct,1,1,32,31);
   for (int x=0; x< NUM_CHARS; x++) {
	    if ((x % 10)==0) {
		   SetRect(&rct,1,1,32,31);
   		   OffsetRect(&rct,0,32*x/10);
	    }
   		m_chars[x] = new CTexture((char *)TEXTFILE,0xFF000000,&rct,32,32);
		OffsetRect(&rct,32,0);
   }
}

void CText::_DeleteGraphics()
{
	for (int x=0; x< NUM_CHARS; x++) {
		SafeDelete(m_chars[x]);
	}
}

void CText::DrawScore(int score, int x, int y)
{
	D3DXVECTOR2 pnt (x,y);
	DrawScore(score,pnt);
}