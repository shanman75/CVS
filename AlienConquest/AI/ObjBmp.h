#pragma once
#include "obj.h"

// This class is for a screen filling bitmap

class CObjBmp :
	public CObj
{
public:
	CObjBmp(char *filename, int colkey=0);
	~CObjBmp(void);
	void move(void);
	BOOL done(void) {return m_done;}
	enum EVENT { FIRE };

	void event(EVENT);
private:
	BOOL m_done;
};
