#pragma once
#include "obj.h"
#include "Texture.h"
//#include "Sound.h"

class CObjEnemyWeapon :
	public CObj
{
public:
	CObjEnemyWeapon(void);
	virtual ~CObjEnemyWeapon(void);
protected:
	virtual void paint();
	void _LoadGraphics();
	void _UnloadGraphics();
	static CTexture *m_regular[1];
	//static CTexture *m_firing[4];

	static int m_graph_init;

	int m_fir_seq;
	CTimer m_ani_time;
	//static CSound *m_regsnd;
	//static int m_snd_played;

	static RECT m_myboundrects[1];
};
