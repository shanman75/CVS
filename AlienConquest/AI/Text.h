#pragma once

#include "texture.h"

#define TEXTFILE "resource/cuddly.bmp"

class CText
{
public:
	CText(void);
	~CText(void);
	void Testdraw(void);
	void DrawString(const char *str, D3DXVECTOR2 pnt);
	void DrawScore(int, D3DXVECTOR2);

protected:
	const static int NUM_CHARS=45;
	CTexture *m_chars[NUM_CHARS];
	static int m_graph_init;
	void _InitGraphics();
	void _DeleteGraphics();
};
