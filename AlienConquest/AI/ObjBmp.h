#pragma once
#include "obj.h"

// This class is for a screen filling bitmap

class CObjBmp :
	public CObj
{
public:
	CObjBmp(char *filename, int colkey=0, BOOL anyenabled=FALSE);
	~CObjBmp(void);
	void move(void);
	BOOL done(void) {return m_done;}
	void SetDone(BOOL m) { m_done = m; }
	enum EVENT { FIRE, ANY };

	void event(EVENT);
private:
	BOOL m_done;
	BOOL m_anyenabled;

	CTexture *m_mytexture;
};
