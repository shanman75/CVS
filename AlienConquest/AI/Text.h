#pragma once

#include "texture.h"

#define TEXTFILE "resource/toolbar/font.png"

class CText
{
public:
	CText(void);
	~CText(void);
	void Testdraw(void);
	void DrawString(const char *str, D3DXVECTOR2 pnt);
	void DrawString(const char *str, int x, int y);
	void DrawScore(int, D3DXVECTOR2);
	void DrawScore(int, int, int);

protected:
	const static int NUM_CHARS=45;
	CTexture *m_chars[NUM_CHARS];
	static int m_graph_init;
	void _InitGraphics();
	void _DeleteGraphics();
};
